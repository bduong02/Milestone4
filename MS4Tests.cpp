// Bryan Duong, Sothaninn Sieng
// Milestone 4 Tests

#include <iostream>
#include <cstdlib> 
#include <string>
#include "db_cxx.h"
#include "SQLParserResult.h"
#include "SQLParser.h"
#include "SQLExec.h"
#include "schema_tables.h"
#include "heap_storage.h"

using namespace std;
using namespace hsql;

void showTableColumnTest(ShowStatement::EntityType type, string tableName){
    if(type == ShowStatement::kColumns){
        if(tableName == "") 
        {
            cout << "No table name given." << endl;
        }
        cout << "Getting columns on table: " << tableName << endl;
        ShowStatement* showStmt = new ShowStatement(ShowStatement::EntityType::kColumns);
        showStmt->tableName = (char*)tableName.c_str();
        SQLExec::execute(showStmt);
        delete showStmt->tableName;
     }
     else {
         if(type == ShowStatement::EntityType::kTables){
            ShowStatement* showStmt = new ShowStatement(ShowStatement::EntityType::kTables);
            SQLExec::execute(showStmt);
          } else {
              if(type == ShowStatement::EntityType::kIndex){
                if(tableName == "") 
                {
                  cout << "No table name given for show index." << endl;  
                }
            
              cout << "Calling show index on table: " << tableName << endl;
              ShowStatement* showStmt = new ShowStatement(ShowStatement::EntityType::kIndex);
              showStmt->tableName = (char*)tableName.c_str();
              SQLExec::execute(showStmt);
              delete showStmt->tableName;
            }
          }
     }
     delete showStmt;
     showStmt = nullptr;
}

void createTableTest()
{
    // creating table named goober
    CreateStatement* createStmt = new CreateStatement(CreateStatement::CreateType::kTable);
    createStmt->tableName = (char*)"goober";
    createStmt->columns = new vector<ColumnDefinition*>();
    createStmt->columns->push_back(new ColumnDefinition((char*)"col1", ColumnDefinition::DataType::TEXT));
    createStmt->columns->push_back(new ColumnDefinition((char*)"col2", ColumnDefinition::DataType::INT));
    createStmt->columns->push_back(new ColumnDefinition((char*)"col3", ColumnDefinition::DataType::INT));
    SQLExec::execute(createStmt);
  
    delete createStmt->columns;
    createStmt->columns = nullptr;
    delete createStmt->tableName;
    delete createStmt;
    createStmt = nullptr;
        
    showTableColumnTest(ShowStatement::kTables, "");
    showTableColumnTest(ShowStatement::kColumns, "goober");
}

void createIndexTest()
{
    // creating an index on goober table on columns col1 and col2
    CreateStatement* createStmt = new CreateStatement(CreateStatement::CreateType::kTable);
    createStmt->tableName = (char*)"goober";
    createStmt->indexType = (char*)"BTREE";
    createStmt->indexName = (char*)"i1";
    createStmt->indexColumns = new vector<char*>();
    createStmt->indexColumns->push_back((char*)"col1");
    createStmt->indexColumns->push_back((char*)"col2");
    SQLExec::execute(createStmt);
    delete createStmt->indexColumns;
    delete createStmt;

    // creating an index on goober table on column col3
    CreateStatement* createStmt2 = new CreateStatement(CreateStatement::CreateType::kTable);
    createStmt2->tableName = (char*)"goober";
    createStmt2->indexType = (char*)"BTREE";
    createStmt2->indexName = (char*)"i2";
    createStmt2->indexColumns = new vector<char*>();
    createStmt2->indexColumns->push_back((char*)"col3");
    SQLExec::execute(createStmt2);
    delete createStmt2->indexColumns;
    delete createStmt2;

    // creating an index on a column that does not exist
    CreateStatement* createStmt3 = new CreateStatement(CreateStatement::CreateType::kTable);
    createStmt3->tableName = (char*)"goober";
    createStmt3->indexType = (char*)"BTREE";
    createStmt3->indexName = (char*)"index";
    createStmt3->indexColumns = new vector<char*>();
    createStmt3->indexColumns->push_back((char*)"col4");
    SQLExec::execute(createStmt3);
    delete createStmt3->indexColumns;
    delete createStmt3;

    showTableColumnTest(ShowStatement::kIndex, "goober");
}

void dropIndexTest()
{
    // dropping index i2 from goober table
    DropStatement* dropStmt = new DropStatement(DropStatement::EntityType::kIndex);
    dropStmt->indexName = (char*)"i2";
    dropStmt->name = (char*)"goober";
    SQLExec::execute(dropStmt);
    delete dropStmt;
}

void dropTableTest()
{
    // testing drop table on goober table
    DropStatement* dropStmt = new DropStatement(DropStatement::EntityType::kTable);
    dropStmt->name = (char*)"goober";
    SQLExec::execute(dropStmt);
    delete dropStmt->name;
    delete dropStmt;

    // testing drop on a table that does not exist
    DropStatement* dropStmt2 = new DropStatement(DropStatement::EntityType::kTable);
    dropStmt2 = new DropStatement(DropStatement::EntityType::kTable);
    dropStmt2->name = (char*)"table";
    SQLExec::execute(dropStmt2);
    delete dropStmt2->name;
    delete dropStmt2;
}

