# 4300-Cheetah

## Description
DB Relation Manager project for CPSC4300 at Seattle U, Spring 2023, Project Cheetah.

## Features
### Milestone 3
Implements Create, Show, and Drop SQL statements for tables. Show statements also display columns.

### Milestone 4
Additional functionality for Create, Show, and Drop SQL statements for indexes.

## Installation
1. Clone the repository on CS1

` git clone https://github.com/BguardiaOpen/4300-Cheetah23SQ.git `

2. Ensure the ` .bash_profile ` path is configured correctly

```
export PATH=/usr/local/db6/bin:$PATH
export LD_LIBRARY_PATH=/usr/local/db6/lib:$LD_LIBRARY_PATH
export PYTHONPATH=/usr/local/db6/lib/site-packages:$PYTHONPATH 
```

## Usage
1. Create a directory to hold the database (first time usage only)
2. Compile the program with ` make `
3. Run the program with ` ./m path_to_database_directory `
    
    * The path must be the path to the directory from the root user@cs1
4. Other ``` make ``` options
    
    * ` make clean `: removes the object code files
    * ` make valgrind `: shows locations of memory leaks (might be necessary to change database directory in ` Makefile `)
5. User input options

    * SQL ` CREATE TABLE ` and ` SELECT ` statements (see example)
    * ` test ` runs the Milestone 4 tests
    * ` quit ` exits the program

## Example

```
SQL> show tables
SHOW TABLES
table_name 
+----------+
"goober" 
successfully returned 1 rows
SQL> show columns from goober
SHOW COLUMNS FROM goober
table_name column_name data_type 
+----------+----------+----------+
"goober" "x" "INT" 
"goober" "y" "INT" 
"goober" "z" "INT" 
successfully returned 3 rows
SQL> create index fx on goober (x,y)
CREATE INDEX fx ON goober USING BTREE (x, y)
created index fx
SQL> show index from goober
SHOW INDEX FROM goober
table_name index_name column_name seq_in_index index_type is_unique 
+----------+----------+----------+----------+----------+----------+
"goober" "fx" "x" 1 "BTREE" true 
"goober" "fx" "y" 2 "BTREE" true 
successfully returned 2 rows
SQL> drop index fx from goober
DROP INDEX fx FROM goober
dropped index fx
SQL> show index from goober
SHOW INDEX FROM goober
table_name index_name column_name seq_in_index index_type is_unique 
+----------+----------+----------+----------+----------+----------+
successfully returned 0 rows
SQL> create index fx on goober (x)
CREATE INDEX fx ON goober USING BTREE (x)
created index fx
SQL> show index from goober
SHOW INDEX FROM goober
table_name index_name column_name seq_in_index index_type is_unique 
+----------+----------+----------+----------+----------+----------+
"goober" "fx" "x" 1 "BTREE" true 
successfully returned 1 rows
SQL> create index fx on goober (y,z)
CREATE INDEX fx ON goober USING BTREE (y, z)
Error: DbRelationError: duplicate index goober fx
SQL> show index from goober
SHOW INDEX FROM goober
table_name index_name column_name seq_in_index index_type is_unique 
+----------+----------+----------+----------+----------+----------+
"goober" "fx" "x" 1 "BTREE" true 
successfully returned 1 rows
SQL> create index fyz on goober (y,z)
CREATE INDEX fyz ON goober USING BTREE (y, z)
created index fyz
SQL> show index from goober
SHOW INDEX FROM goober
table_name index_name column_name seq_in_index index_type is_unique 
+----------+----------+----------+----------+----------+----------+
"goober" "fx" "x" 1 "BTREE" true 
"goober" "fyz" "y" 1 "BTREE" true 
"goober" "fyz" "z" 2 "BTREE" true 
successfully returned 3 rows
SQL> drop index fx from goober
DROP INDEX fx FROM goober
dropped index fx
SQL> show index from goober
SHOW INDEX FROM goober
table_name index_name column_name seq_in_index index_type is_unique 
+----------+----------+----------+----------+----------+----------+
"goober" "fyz" "y" 1 "BTREE" true 
"goober" "fyz" "z" 2 "BTREE" true 
successfully returned 2 rows
SQL> drop index fyz from goober
DROP INDEX fyz FROM goober
dropped index fyz
SQL> show index from goober
SHOW INDEX FROM goober
table_name index_name column_name seq_in_index index_type is_unique 
+----------+----------+----------+----------+----------+----------+
successfully returned 0 rows
```

## Acknowledgements
* [Berkeley DB](https://www.oracle.com/database/technologies/related/berkeleydb.html)
* [Berkeley DB Dbt](https://docs.oracle.com/cd/E17076_05/html/api_reference/CXX/frame_main.html)
* [Hyrise SQL Parser](https://github.com/klundeen/sql-parser)
* [Professor Lundeen's 5300-Instructor base code](https://github.com/klundeen/5300-Instructor/releases/tag/Milestone2h) 🙏🙏
* [The Python equivalent](https://github.com/BguardiaOpen/cpsc4300py)
