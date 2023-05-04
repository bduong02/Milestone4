/**
 * @file SQLExec.cpp - implementation of SQLExec class
 * @author Bryan Duong, Sothaninn Sieng
 */
#include "SQLExec.h"

using namespace std;
using namespace hsql;

// define static data
Tables *SQLExec::tables = nullptr;

// make query result be printable
ostream &operator<<(ostream &out, const QueryResult &qres) {
    if (qres.column_names != nullptr) {
        for (auto const &column_name: *qres.column_names)
            out << column_name << " ";
        out << endl << "+";
        for (unsigned int i = 0; i < qres.column_names->size(); i++)
            out << "----------+";
        out << endl;
        for (auto const &row: *qres.rows) {
            for (auto const &column_name: *qres.column_names) {
                Value value = row->at(column_name);
                switch (value.data_type) {
                    case ColumnAttribute::INT:
                        out << value.n;
                        break;
                    case ColumnAttribute::TEXT:
                        out << "\"" << value.s << "\"";
                        break;
                    default:
                        out << "???";
                }
                out << " ";
            }
            out << endl;
        }
    }
    out << qres.message;
    return out;
}

QueryResult::~QueryResult() {
    if(this->column_names) {
        delete this->column_names;
    }
    if(this->column_attributes) {
        delete this->column_attributes;
    }
    if(this->rows) {
        delete this->rows;
    }
}

QueryResult *SQLExec::execute(const SQLStatement *statement) {
    // checking if tables have been initialized
    // and creating tables if not
    if(SQLExec::tables == nullptr)
    {
        SQLExec::tables = new Tables();
    }
    // checking if indices have been initiated
    // and creating indices if not
    if(SQLExec::indices == nullptr)
    {
        SQLExec::indices = new Indices();
    }

    try {
        switch (statement->type()) {
            case kStmtCreate:
                return create((const CreateStatement *) statement);
            case kStmtDrop:
                return drop((const DropStatement *) statement);
            case kStmtShow:
                return show((const ShowStatement *) statement);
            default:
                return new QueryResult("not implemented");
        }
    } catch (DbRelationError &e) {
        throw SQLExecError(string("DbRelationError: ") + e.what());
    }
}

void SQLExec::column_definition(const ColumnDefinition *col, Identifier &column_name, ColumnAttribute &column_attribute) {
   column_name = col->name;
   switch(col->type)
   {
        case ColumnDefinition::DataType::INT:
            column_attribute = ColumnDefinition::DataType::INT;
            break;
        case ColumnDefinition::DataType::TEXT:
            column_attribute = ColumnAttribute::DataType::TEXT;
            break;
        default:
            throw SQLExecError("not implemented");
   }    
}
// CREATE ...
QueryResult *SQLExec::create(const CreateStatement *statement) {
    switch(statement->type)
    {
        case CreateStatement::kIndex:
            return create_index(statement);
        case CreateStatement::kTable:
            return create_table(statement);
        default:
            return new QueryResult("not implemented");
    }
}

// DROP ...
QueryResult *SQLExec::drop(const DropStatement *statement) {
    switch(statement->type)
    {
        case DropStatement::kIndex:
            return drop_index(statement);
        case DropStatement::kTable:
            return drop_table(statement);
        default:
            return new QueryResult("not implemented");
    }
}
// SHOW ...
QueryResult *SQLExec::show(const ShowStatement *statement) {
    switch (statement-> type)
    {
        case ShowStatement::kIndex:
            return show_index(statement);
        case ShowStatement::kTables:
            return show_tables();
        case ShowStatement::kColumns:
            return show_columns(statement);
        default:
            return new QueryResult("not implemented");
}
//Show the tables in _tables table
QueryResult *SQLExec::show_tables() {
    ColumnNames *col_names = new ColumnNames;
    col_names->push_back("table_name");
    ColumnAttributes *col_attrib = new ColumnAttributes;
    col_attrib->push_back(ColumnAttribute(ColumnAttribute::TEXT));
    
    //Select all the record of _tables table 
    Handles *handles = SQLExec::tables->select();
    //Number of rows excluding the name of the _tables table, _columns tables and _indices table
    u_long n = handles->size() - 3;

    //For each record in _tables table, extract row data(row_dict) by column names
    ValueDicts *rows = new ValueDicts;
    for (auto const &handle: *handles) {
        ValueDict *row_dict = SQLExec::tables->project(handle, col_names);
        Identifier table_name = row_dict->at("table_name").s;
        //Don't show if table_name is the name of the _tables table, _columns table or _indices table, 
        if (table_name != Tables::TABLE_NAME && table_name != Columns::TABLE_NAME && table_name != Indices::TABLE_NAME)
            rows->push_back(row_dict);
        else
            delete row_dict;
    }
    delete handles;
    return new QueryResult(col_names, col_attrib, rows, "successfully returned " + to_string(n) + " rows");
}
//Show the columns in _columns table
QueryResult *SQLExec::show_columns(const ShowStatement *statement) {
    //Get _columns table 
    DbRelation &columns = SQLExec::tables->get_table(Columns::TABLE_NAME);

    ColumnNames *col_names = new ColumnNames;
    col_names->push_back("table_name");
    col_names->push_back("column_name");
    col_names->push_back("data_type");
    ColumnAttributes *col_attrib = new ColumnAttributes;
    col_attrib->push_back(ColumnAttribute(ColumnAttribute::TEXT));

    ValueDict where;
    where["table_name"] = Value(statement->tableName);
    //Select from the _columns table where table_name = tableName in statement
    Handles *handles = columns.select(&where);
    //number of rows returned
    u_long n = handles->size();
    
    //For each record in _columns table, extract row data(row_dict) by table_name, column_name and data_type
    ValueDicts *rows = new ValueDicts;
    for (auto const &handle: *handles) {
        ValueDict *row_dict = columns.project(handle, col_names);
        rows->push_back(row_dict);
    }
    delete handles;
    return new QueryResult(col_names, col_attrib, rows, "successfully returned " + to_string(n) + " rows");
}

static QueryResult *SQLExec::create_index(const hsql::CreateStatement *statement){
    return new QueryResult("not implemented");
}
    
static QueryResult *SQLExec::drop_index(const hsql::DropStatement *statement){
    return new QueryResult("not implemented");
}

QueryResult *SQLExec::show_index(const hsql::ShowStatement *statement){
    return new QueryResult("not implemented");
}
