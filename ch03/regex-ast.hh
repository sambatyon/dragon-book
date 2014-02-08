#pragma once

#include <unordered_set>
#include <memory>
#include <cstdint>

enum AstNodeType {
  eAstNodeSymbol,
  eAstNodeStar,
  eAstNodeEither,
  eAstNodeCat
};

// This part should have visitors and all those things

class AstNode;
typedef std::shared_ptr<AstNode> AstNodePtr;

class AstNode {
 public:
  AstNode(AstNodeType type);
  virtual ~AstNode();

  AstNodeType type() const {return type_;}
  void set_type(AstNodeType type) {type_ = type;}

  bool nullable() const {return nullable_;}
  std::unordered_set<std::uint32_t> const &first_positions() const {
    return first_positions_;
  }
  std::unordered_set<std::uint32_t> const &last_positions() const {
    return last_positions_;
  }

  AstNode *parent() const {return parent_;}
  void set_parent(AstNode *parent) {parent_ = parent;}

  virtual void Update() = 0;

 protected:
  AstNodeType type_;
  bool nullable_;
  std::unordered_set<std::uint32_t> first_positions_;
  std::unordered_set<std::uint32_t> last_positions_;
  AstNode *parent_;
};

class AstNodeSymbol : public AstNode {
 public:
  AstNodeSymbol();
  AstNodeSymbol(char symbol, std::uint32_t pos);

  char symbol() const {return symbol_;}
  void set_symbol(char symbol);

  std::uint32_t position() {return position_;}
  void set_position(std::uint32_t position);

 protected:
  char symbol_;
  std::uint32_t position_;

  void Update() override {
    DoUpdate();
  }
  void DoUpdate();
};

class AstNodeStar : public AstNode {
 public:
  AstNodeStar();
  AstNodeStar(AstNodePtr ast);
  ~AstNodeStar();

  AstNodePtr ast() const {return ast_;}
  void set_ast(AstNodePtr ast);

 protected:
  AstNodePtr ast_;

  void Update() override {
    DoUpdate();
  }
  void DoUpdate();
};

class AstNodeEither : public AstNode {
 public:
  AstNodeEither();
  AstNodeEither(AstNodePtr left, AstNodePtr right);
  ~AstNodeEither();

  AstNodePtr left() const {return left_;}
  void set_left(AstNodePtr left);

  AstNodePtr right() const {return right_;}
  void set_right(AstNodePtr right);

 protected:
  AstNodePtr left_;
  AstNodePtr right_;

  void Update() override {
    DoUpdate();
  }
  void DoUpdate();
};

class AstNodeCat : public AstNode {
 public:
  AstNodeCat();
  AstNodeCat(AstNodePtr left, AstNodePtr right);
  ~AstNodeCat();

  AstNodePtr left() const {return left_;}
  void set_left(AstNodePtr left);

  AstNodePtr right() const {return right_;}
  void set_right(AstNodePtr right);

 protected:
  AstNodePtr left_;
  AstNodePtr right_;

  void Update() override {
    DoUpdate();
  }
  void DoUpdate();
};
