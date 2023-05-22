//Authors: Ishan Parikh, Ryan S.
//Date:    5-21-23
//Purpose: implementation of a sql evalutatoin plan class 
//Doc:
// --ptrs are smartptr in class: slight performance hit but significantly abstracts
//   implementation

#ifndef EVAL_PLAN
#define EVAL_PLAN

#include "storage_engine.h"
#include <memory>

typedef std::pair<DbRelation *, Handles *> EvalPipeline;

class EvalPlan {
    public:
        enum PlanType {
            PROJECTALL, PROJECT, SELECT, TABLESCAN
        };

        
        EvalPlan(PlanType type, EvalPlan *relation);  // use for ProjectAll, e.g., EvalPlan(EvalPlan::ProjectAll, table);
        EvalPlan(ColumnNames *projection, EvalPlan *relation); // use for Project
        EvalPlan(ValueDict *conjunction, EvalPlan *relation);  // use for Select
        EvalPlan(DbRelation &table);  // use for TableScan
        EvalPlan(const EvalPlan *other);  // use for copying
        virtual ~EvalPlan();

        // Attempt to get the best equivalent evaluation plan
        EvalPlan *optimize();

        // Evaluate the plan: evaluate gets values, pipeline gets handles
        ValueDicts *evaluate();

        EvalPipeline pipeline();

    protected:

        PlanType type;                  //what's the tyep of plan needed

        //use smart ptrs to avoid the headache of constantly managing memory
        EvalPlan* relation;             // for everything except TableScan
        ColumnNames* projection;        // columns to project
        ValueDict* select_conjunction;  // values gotten from select
        DbRelation &table;              // for TableScan
};

#endif