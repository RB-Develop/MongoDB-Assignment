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
 
void insertPerson(mongo::DBClientConnection & conn,  const char * name,  const char * age, mongo::BSONObj street, mongo::BSONObj history) {
    mongo::BSONObjBuilder obj;
    obj.append("name" , name);
	obj.append("age", age);
	obj.append("address", street);
	obj.append("history", history);
    conn.insert( "test.people" , obj.obj() );
}

mongo::BSONObj createStreetBSON(const char * street, const char * city, const char * state, const char * zip) {
	mongo::BSONObjBuilder obj;
	obj.append("street", street);
	obj.append("city", city);
	obj.append("state", state);
	obj.append("zip", zip);
	return obj.obj();
}

mongo::BSONObj createHistoryBSON(const bool criminal, const bool unemployed, const bool hopeless) {
	mongo::BSONObjBuilder obj;
	obj.append("criminal", criminal);
	obj.append("unemployed", unemployed);
	obj.append("hopeless", hopeless);
	return obj.obj();
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
 
    insertPerson(conn, "Joe", "33", createStreetBSON("123 Fake Street", "Faketon", "MA", "12345"), createHistoryBSON(false, true, true));
	insertPerson(conn, "Bob", "14", createStreetBSON("Fakeadilly Street", "Faketon", "BB", "5554"), createHistoryBSON(true, false, false));
	insertPerson(conn, "Claptrap", "Undetermined", createStreetBSON("Swagadilly", "Amsterfake", "NH", "15643"), createHistoryBSON(false, false, true));

    {
        mongo::BSONObj query = BSON("address.city" << "Amsterfake" );
        cout << "Current query: " << query << endl;
 
        auto_ptr<mongo::DBClientCursor> cursor = conn.query( "test.people" , query );
        cout << "using cursor" << endl;
        while ( cursor->more() ) {
            mongo::BSONObj obj = cursor->next();
			cout << "Data: \n" << obj.jsonString(mongo::Strict, true) << endl;
        }
 
    }
}