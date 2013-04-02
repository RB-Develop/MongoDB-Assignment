/*    Copyright 2009 10gen Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
 
/**
 * this is a good first example of how to use mongo from c++
 */
 
#include <iostream>
 
#include "mongo/client/dbclient.h"
 
using namespace std;
 
void insert( mongo::DBClientConnection & conn , const char * name , int num ) {
    mongo::BSONObjBuilder obj;
    obj.append( "name" , name );
    obj.append( "num" , num );
    conn.insert( "test.people" , obj.obj() );
}
 
int main( int argc, const char **argv ) {
 
    const char *port = "27017";
    if ( argc != 1 ) {
        if ( argc != 3 )
            throw -12;
        port = argv[ 2 ];
    }
 
    mongo::DBClientConnection conn;
    string errmsg;
    if ( ! conn.connect( string( "127.0.0.1:" ) + port , errmsg ) ) {
        cout << "couldn't connect : " << errmsg << endl;
        throw -11;
    }
 
    {
        // clean up old data from any previous tests
        mongo::BSONObjBuilder query;
        conn.remove( "test.people" , query.obj() );
    }
 
    insert( conn, "a", 1 );
    insert( conn, "a", 2 );
    insert( conn, "a", 3 );
    insert( conn, "a", 4 );
    insert( conn, "a", 5 );
    insert( conn, "a", 6 );
    insert( conn, "a", 7 );
    insert( conn, "a", 8 );
    insert( conn, "a", 9 );
    insert( conn, "a", 10 );
 
    {
        mongo::BSONObj query = BSON( "num" << BSON( "$nin" << BSON_ARRAY(1 << 5 << 10) ) );
        cout << query << endl;
 
        auto_ptr<mongo::DBClientCursor> cursor = conn.query( "test.people" , query );
        cout << "using cursor" << endl;
        while ( cursor->more() ) {
            mongo::BSONObj obj = cursor->next();
            cout << "\t" << obj.jsonString() << endl;
        }
 
    }
}