/**
 * @file SQLExec.cpp - implementation of SQLExec class
 * @author Bryan Duong, Ninn Sieng
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

QueryResult *SQLExec::show(const ShowStatement *statement) {
    return new QueryResult("not implemented"); // FIXME
}

QueryResult *SQLExec::show_tables() {
    return new QueryResult("not implemented"); // FIXME
}

QueryResult *SQLExec::show_columns(const ShowStatement *statement) {
    return new QueryResult("not implemented"); // FIXME
}

