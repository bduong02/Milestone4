#include "EvalPlan.h"



EvalPlan::EvalPlan(PlanType type, EvalPlan* relation) {
  this->type = type;
  this->relation = relation;
  this->projection = nullptr;
  this->select_conjunction = nullptr;
  this->table = DbRelation();
}

EvalPlan::EvalPlan(ColumnNames *projection, EvalPlan *relation) {
  const PlanType EVAL_TYPE = PROJECT;

  this->type = EVAL_TYPE;
  this->relation = relation;
  this->projection = projection;
  this->select_conjunction = nullptr;
  this->table = DbRelation();
}

EvalPlan::EvalPlan(ValueDict *conjunction, EvalPlan *relation) {
  const PlanType EVAL_TYPE = SELECT;

  this->type = EVAL_TYPE;
  this->relation = relation;
  this->projection = nullptr;
  this->select_conjunction = conjunction;
  this->table = DbRelation();
}

EvalPlan::EvalPlan(DbRelation &table) {
  
}


EvalPlan::EvalPlan(const EvalPlan* other) {
  this->type = other->type;
  this->table = other->table;
  this->relation = (other->relation? new EvalPlan(*other->relation) : nullptr);
  this->projection = (other->projection ? new ColumnNames(*other->projection) : nullptr);
  this->select_conjunction(other->select_conjunction ? new ValueDict(*other->select_conjunction) : nullptr);
}

EvalPlan::~EvalPlan() {
  if(relation) delete relation;
  if(projection) delete projection;
  if(select_conjunction) delete select_conjunction;
}

