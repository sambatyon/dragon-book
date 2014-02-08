#include "regex-ast.hh"

AstNode::AstNode(AstNodeType type) : type_(type)
                                   , nullable_(true)
                                   , first_positions_()
                                   , last_positions_()
                                   , parent_(nullptr) {

}

AstNode::~AstNode() {

}

AstNodeSymbol::AstNodeSymbol() : AstNodeSymbol('\x19', 0) {

}

AstNodeSymbol::AstNodeSymbol(char symbol, std::uint32_t pos)
    : AstNode(eAstNodeSymbol)
    , symbol_(symbol)
    , position_(pos) {
  DoUpdate();
}

void AstNodeSymbol::DoUpdate() {
  nullable_ = symbol_ == '\x19' ? true : false;
  first_positions_.clear();
  last_positions_.clear();
  if (!nullable_) {
    first_positions_.insert(position_);
    last_positions_.insert(position_);
  }
  if (parent_)
    parent_->Update();
}

void AstNodeSymbol::set_symbol(char symbol) {
  symbol_ = symbol;
  Update();
}

void AstNodeSymbol::set_position(std::uint32_t position) {
  position_ = position;
  Update();
}


AstNodeStar::AstNodeStar() : AstNodeStar(AstNodePtr()) {

}

AstNodeStar::AstNodeStar(AstNodePtr ast) : AstNode(eAstNodeStar), ast_(ast) {
  if (ast_)
    ast_->set_parent(this);
  DoUpdate();
}

AstNodeStar::~AstNodeStar() {
  if (ast_ && ast_->parent() == this)
    ast_->set_parent(nullptr);
}

void AstNodeStar::set_ast(AstNodePtr ast) {
  if (ast_)
    ast_->set_parent(nullptr);
  ast_ = ast;
  if (ast_)
    ast->set_parent(this);
  Update();
}

void AstNodeStar::DoUpdate() {
  nullable_ = true;
  first_positions_.clear();
  last_positions_.clear();
  if (ast_) {
    first_positions_.insert(std::begin(ast_->first_positions()),
                            std::end(ast_->first_positions()));
    last_positions_.insert(std::begin(ast_->last_positions()),
                           std::end(ast_->last_positions()));
  }
  if (parent_)
    parent_->Update();
}

AstNodeEither::AstNodeEither() : AstNodeEither(AstNodePtr(), AstNodePtr()) {

}

AstNodeEither::AstNodeEither(AstNodePtr left, AstNodePtr right)
    : AstNode(eAstNodeEither)
    , left_(left)
    , right_(right) {
  if (left_)
    left_->set_parent(this);
  if (right_)
    right_->set_parent(this);
  DoUpdate();
}

AstNodeEither::~AstNodeEither() {
  if (left_ && left_->parent() == this)
    left_->set_parent(nullptr);
  if (right_ && right_->parent() == this)
    right_->set_parent(nullptr);
}

void AstNodeEither::set_left(AstNodePtr left) {
  if (left_)
    left_->set_parent(nullptr);
  left_ = left;
  if (left_)
    left->set_parent(this);
  Update();
}

void AstNodeEither::set_right(AstNodePtr right) {
  if (right_)
    right_->set_parent(nullptr);
  right_ = right;
  if (right_)
    right->set_parent(this);
  Update();
}

void AstNodeEither::DoUpdate() {
  first_positions_.clear();
  last_positions_.clear();

  if (left_ && right_) {
    nullable_ = left_->nullable() || right_->nullable();

    first_positions_.insert(std::begin(left_->first_positions()),
                            std::end(left_->first_positions()));
    first_positions_.insert(std::begin(right_->first_positions()),
                            std::end(right_->first_positions()));

    last_positions_.insert(std::begin(left_->last_positions()),
                           std::end(left_->last_positions()));
    last_positions_.insert(std::begin(right_->last_positions()),
                           std::end(right_->last_positions()));
  } else {
    nullable_ = true;
  }

  if (parent_)
    parent_->Update();
}

AstNodeCat::AstNodeCat() : AstNodeCat(AstNodePtr(), AstNodePtr()) {

}

AstNodeCat::AstNodeCat(AstNodePtr left, AstNodePtr right)
    : AstNode(eAstNodeCat)
    , left_(left)
    , right_(right) {
  if (left_)
    left_->set_parent(this);
  if (right_)
    right_->set_parent(this);
  DoUpdate();
}

AstNodeCat::~AstNodeCat() {
  if (left_ && left_->parent() == this)
    left_->set_parent(nullptr);
  if (right_ && right_->parent() == this)
    right_->set_parent(nullptr);
}

void AstNodeCat::set_left(AstNodePtr left) {
  if (left_)
    left_->set_parent(nullptr);
  left_ = left;
  if (left_)
    left_->set_parent(this);
  Update();
}

void AstNodeCat::set_right(AstNodePtr right) {
  if (right_)
    right_->set_parent(nullptr);
  right_ = right;
  if (right_)
    right_->set_parent(this);
  Update();
}

void AstNodeCat::DoUpdate() {
  first_positions_.clear();
  last_positions_.clear();

  if (left_ && right_) {
    nullable_ = left_->nullable() && right_->nullable();

    first_positions_.insert(std::begin(left_->first_positions()),
                            std::end(left_->first_positions()));
    if (left_->nullable())
      first_positions_.insert(std::begin(right_->first_positions()),
                              std::end(right_->first_positions()));

    if (right_->nullable())
      last_positions_.insert(std::begin(left_->last_positions()),
                             std::end(left_->last_positions()));
    last_positions_.insert(std::begin(right_->last_positions()),
                           std::end(right_->last_positions()));
  } else {
    nullable_ = true;
  }

  if (parent_)
    parent_->Update();
}
