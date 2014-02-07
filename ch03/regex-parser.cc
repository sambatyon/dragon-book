#include "regex-parser.hh"

RegexParser::RegexParser(RegexLexer &lexer) : lexer_(lexer)
                                            , lookahead_()
                                            , state_id_gen_(0) {
  Move();
}

void RegexParser::Move() {
  lookahead_ = lexer_.Scan();
}

NFiniteAutomataPtr RegexParser::Parse() {
  return Regexes(); // if the tag after this return is different from kEof,
                    // there was an error
}

NFiniteAutomataPtr RegexParser::Regex() {
  switch (lookahead_.tag) {
    case RegexLexer::kEof:
      // This should not happen
      return NFiniteAutomataPtr(); // nothing to be done
      break;
    case RegexLexer::kLParen:
      return CheckModifiers(ParenRegex());
      break;
    case RegexLexer::kSymbol: {

      auto res = std::make_shared<NFiniteAutomata>();
      auto initial_state = state_id_gen_++;
      res->states().insert(initial_state);
      res->set_initial_state(initial_state);
      auto final_state = state_id_gen_++;
      res->states().insert(final_state);
      res->accepting_states().insert(final_state);
      res->AddTransition(initial_state, lookahead_.value, final_state);

      Move();
      return CheckModifiers(res);
    } break;
    case RegexLexer::kRParen:
    case RegexLexer::kKleene:
    case RegexLexer::kEither:
    default:
      throw std::runtime_error("syntax error");
      break;
  }
}

NFiniteAutomataPtr RegexParser::CheckModifiers(NFiniteAutomataPtr automata) {
  NFiniteAutomataPtr res = automata;
  if (lookahead_.tag == RegexLexer::kKleene) {
    Move();
    res = ApplyKleene(automata);
  } else if (lookahead_.tag == RegexLexer::kEither) {
    Move();
    auto right_hand = Regex();
    res = ApplyOr(res, right_hand);
  }
  return res;
}

NFiniteAutomataPtr RegexParser::Regexes(bool could_have_end_paren) {
  if (lookahead_.tag == RegexLexer::kEof)
    return NFiniteAutomataPtr();
  auto first = Regex();
  // If we reach this place from a parenthesis, it can be done after the first
  // regex and then the next symbol is a closing parenthesis. Any other symbol
  // should have been consumed in the previous call to regex. If there is
  // something like a** then there's a syntax error and the next call to
  // regex will handle it.
  if (lookahead_.tag == RegexLexer::kEof ||
       (could_have_end_paren && lookahead_.tag == RegexLexer::kRParen))
    return first;
  auto second = Regexes();
  return ApplyConcat(first, second);
}

NFiniteAutomataPtr RegexParser::ParenRegex() {
  Match(RegexLexer::kLParen);
  auto res = Regexes(true);
  // There must be no empty regex in a parenthesis
  if (!res)
      throw std::runtime_error("syntax error");
  Match(RegexLexer::kRParen);
  return res;
}

NFiniteAutomataPtr RegexParser::ApplyKleene(NFiniteAutomataPtr automata) {
  for (auto &accepting : automata->accepting_states())
    automata->AddTransition(accepting, NFiniteAutomata::kEpsilon,
                            automata->initial_state());

  auto initial_state = state_id_gen_++;
  automata->states().insert(initial_state);
  automata->AddTransition(initial_state, NFiniteAutomata::kEpsilon,
                          automata->initial_state());
  automata->set_initial_state(initial_state);

  auto final_state = state_id_gen_++;
  automata->states().insert(final_state);
  for (auto &accepting : automata->accepting_states())
    automata->AddTransition(accepting, NFiniteAutomata::kEpsilon,
                            final_state);
  automata->accepting_states().clear();
  automata->accepting_states().insert(final_state);

  automata->AddTransition(initial_state, NFiniteAutomata::kEpsilon,
                          final_state);

  return automata;
}

NFiniteAutomataPtr RegexParser::ApplyOr(NFiniteAutomataPtr left,
                                        NFiniteAutomataPtr right) {
  auto res = std::make_shared<NFiniteAutomata>();

  auto initial_state = state_id_gen_++;
  res->states().insert(initial_state);
  res->set_initial_state(initial_state);

  auto final_state = state_id_gen_++;
  res->states().insert(final_state);
  res->accepting_states().insert(final_state);

  res->states().insert(std::begin(left->states()), std::end(left->states()));
  res->transitions_.insert(std::begin(left->transitions_),
                           std::end(left->transitions_));
  res->AddTransition(initial_state, NFiniteAutomata::kEpsilon,
                     left->initial_state());
  for (auto &accepting : left->accepting_states())
    res->AddTransition(accepting, NFiniteAutomata::kEpsilon,
                       final_state);

  res->states().insert(std::begin(right->states()), std::end(right->states()));
  res->transitions_.insert(std::begin(right->transitions_),
                           std::end(right->transitions_));
  res->AddTransition(initial_state, NFiniteAutomata::kEpsilon,
                     right->initial_state());
  for (auto &accepting : right->accepting_states())
    res->AddTransition(accepting, NFiniteAutomata::kEpsilon,
                       final_state);

  return res;
}

NFiniteAutomataPtr RegexParser::ApplyConcat(NFiniteAutomataPtr left,
                                            NFiniteAutomataPtr right) {
  auto res = std::make_shared<NFiniteAutomata>();

  res->states().insert(std::begin(left->states()), std::end(left->states()));
  res->transitions_.insert(std::begin(left->transitions_),
                           std::end(left->transitions_));
  res->set_initial_state(left->initial_state());

  res->states().insert(std::begin(right->states()), std::end(right->states()));
  res->transitions_.insert(std::begin(right->transitions_),
                           std::end(right->transitions_));
  res->accepting_states().insert(std::begin(right->accepting_states()),
                                 std::end(right->accepting_states()));

  auto initial_right = right->initial_state();
  std::map<std::tuple<state_t, char>, state_set_t> initial_right_transitions;
  for (auto &right_transition : right->transitions_)
    if (std::get<0>(right_transition.first) == initial_right)
      initial_right_transitions.insert(right_transition); // unecesary copy of
                                                           // sets

  for (auto &left_accepting : left->accepting_states()) {
    for (auto &right_transition : initial_right_transitions)
      for (auto &goal_state : right_transition.second)
        res->AddTransition(left_accepting, std::get<1>(right_transition.first),
                           goal_state);
  }

  for (auto &right_transition : initial_right_transitions){
    for (auto &goal_state : right_transition.second){
      res->RemoveTransition(std::get<0>(right_transition.first),
                            std::get<1>(right_transition.first),
                            goal_state);
    }
  }
  res->states().erase(right->initial_state());

  return res;
}

void RegexParser::Match(const RegexLexer::token_tag_t tag) {
    if (lookahead_.tag == tag)
        Move();
    else
        throw std::runtime_error("syntax error");
}
