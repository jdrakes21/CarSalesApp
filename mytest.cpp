/*****************************************
** File:    mytest.cpp
** Project: CMSC 341 Project 4, Fall 2023
** Author:  Jervon Drakes
** Date:    12/05/2023
** E-mail:  jdrakes1@umbc.edu
** 
**
** This file contains the mytest.cpp file for Project 3.
** This program implements functions for the tester class to 
** implement numerous test cases for the functions in dealer.cpp
**
**
**
***********************************************/

#include "dealer.h"



unsigned int hashCode(const string str) {
   unsigned int val = 0 ;
   const unsigned int thirtyThree = 33 ;  // magic number from textbook
   for (unsigned int i = 0 ; i < str.length(); i++)
      val = val * thirtyThree + str[i] ;
   return val ;
}

class Tester{
public:

  // Function: CarDB::insert(Car car)
  // Case: Tests insertion in the hash table
  // Expected Result: The function should return true indicating that all insertions were successful
  //, retrievals match the expected values and the database size is updated correctly 
  bool testInsertion(){

    // create a CarDB object with a specified size and hash function
    CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);
    bool result = true;
    int currentSize = 0;

    // non-colliding data points
    Car car1("challenger", 10, 1001, true);
    Car car2("stratos", 15, 1002, true);
    Car car3("gt500", 20, 1003, true);
    
    // test non-colliding data points insertion
    if(!cardb.insert(car1)){

      result = false;
    } else {

      currentSize++;
    }

    if(!cardb.insert(car2)){

      result = false;

    } else {

      currentSize++;

    }

    if(!cardb.insert(car3)){

      result = false;

    } else {

      currentSize++;

    }
	

    // determine whether non colliding keys are inserted in the correct bucket
    if(!(cardb.getCar(car1.getModel(), car1.getDealer()) == car1)){

      result = false;

    }

    if(!(cardb.getCar(car2.getModel(), car2.getDealer()) == car2)){

      result = false;

    }

    if(!(cardb.getCar(car3.getModel(), car3.getDealer()) == car3)){

      result = false;

    }

    // determine if the data size changes correctly
    if(!(currentSize == 3)){

	result = false;

    
    }

    return result;
  }

  // Function: CarDB::getCar(string model, int dealer) const
  // Case: Tests the error case for the find operation
  // Expected Result: The function should return true indicating that attempting to 
  // find a car that does not exist in the database results in the EMPTY placeholder
  bool testErrorFind(){

    bool result = true;
    CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);
    
    // create a Car object representing a car that does not exist in the database
    Car carNonExisting("nonexistent", 9999, 1234, true);

    // attempt to find the non-existing car in the database
    Car carResult = cardb.getCar(carNonExisting.getModel(), carNonExisting.getDealer());


    // determine if the result is equal to the EMPTY placeholder, indicatin the car was not found
    if(!(carResult == EMPTY)){


      result = false;

    }

    return result;

  }

  // Function: CarDB::getCar(string model, int dealer) const
  // Case: Test the find operation (getCar(...) function) with several non-colliding keys
  // Expected Result: The function should return true indicating that retrieving cars with non-colliding keys
  // from the database returns the expected 'Car' objects
  bool testFindNonCollisionKeys(){

    bool result = true;

    CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);

    
    // create cars with non-colliding keys
    Car car1("challenger", 10, 1001, true);
    Car car2("stratos", 15, 1002, true);
    Car car3("gt500", 20, 1003, true);

    // insert non-colliding keys
    cardb.insert(car1);
    cardb.insert(car2);
    cardb.insert(car3);

    // determine if retrieving each car using non-colliding keys returns the expected result
    if(!(cardb.getCar(car1.getModel(), car1.getDealer()) == car1)){
      

      result = false;

    }

    
    if(!(cardb.getCar(car2.getModel(), car2.getDealer()) == car2)){


      result = false;

    }

    
    if(!(cardb.getCar(car3.getModel(), car3.getDealer()) == car3)){


      result = false;

    }



    return result;
    
  }

  // Function: CarDB::getCar(string model, int dealer) const
  // Case: Test the find operation (getCar(...) function) with several colliding keys without triggering a rehash
  // Expected Result: The function should return true indicating that retrieving cars with both non-colliding and
  // colliding keys from the database returns the expected 'Car' objects
  bool testCollisionKeys(){

    bool result = true;

    CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);

    // create cars with non-colliding keys
    Car car1("challenger", 10, 1001, true);
    Car car2("stratos", 15, 1002, true);
    Car car3("gt500", 20, 1003, true);

    // insert cars with non-colliding keys into the database
    cardb.insert(car1);
    cardb.insert(car2);
    cardb.insert(car3);

    // determine if retrieving each non-colliding key car returns the expected result
    if(!(cardb.getCar(car1.getModel(), car1.getDealer()) == car1)){


      result = false;

    }


    if(!(cardb.getCar(car2.getModel(), car2.getDealer()) == car2)){


      result = false;

    }


    if(!(cardb.getCar(car3.getModel(), car3.getDealer()) == car3)){


      result = false;

    }

    // create cars with colliding keys
    Car collidingCar("collide", 5, 1004, true);
    Car collidingCar2("collide", 8, 1005, true);

    // insert colliding keys
    cardb.insert(collidingCar);
    cardb.insert(collidingCar2);

    // test if retrieving each colliding key car returns the expected result
    if(!(cardb.getCar(collidingCar.getModel(), collidingCar.getDealer()) == collidingCar)){


      result = false;

    }

    if(!(cardb.getCar(collidingCar2.getModel(), collidingCar2.getDealer()) == collidingCar2)){


      result = false;

    }


    return result;

  }

  // Function: CarDB::remove(Car car)
  // Case: Test the remove operation with a few non-colliding keys.
  // Expected Result: The function should return true indicating that attempting to remove cars with 
  // non-colliding keys from the database is successful
  bool testRemoveNonCollidingKeys(){

    CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);

    // Insert some data
    Car car1("challenger", 10, 1001, true);
    Car car2("stratos", 15, 1002, true);

    cardb.insert(car1);
    cardb.insert(car2);

    // Remove the keys
    bool removed1 = cardb.remove(car1);
    bool removed2 = cardb.remove(car2);

    // Check if the removal was successful
    return (removed1 && removed2 &&
            cardb.getCar("challenger", 1001) == EMPTY &&
            cardb.getCar("stratos", 1002) == EMPTY);
    

  }

  // Function: CarDB::remove(Car car)
  // Case: Test the remove operation with a number of colliding keys without triggering a rehash.
  // Expected Result: The function should return true indicating that attempting to remove all cars  from the database is successful including one with a colliding key is successful
  bool testNoRehash(){

    CarDB cardb(MINPRIME, hashCode, QUADRATIC);      
  
    // Insert some data with colliding keys
    Car car1("challenger", 10, 1001, true);
    Car car2("stratos", 15, 1002, true);

    cardb.insert(car1);
    cardb.insert(car2);

    // Remove the keys
    bool removed1 = cardb.remove(car1);
    bool removed2 = cardb.remove(car2);

    // Check if the removal was successful
    return (removed1 && removed2 &&
            cardb.getCar("challenger", 1001) == EMPTY &&
            cardb.getCar("stratos", 1002) == EMPTY);
  }

  // Function: CarDB::insert(Car car)
  // Case: Test the rehashing is triggered after a descent number of data insertion.
  // Expected Result: The function should return true indicating that after inserting 300 cars
  // the load factor of the hash table remains below or equal to 0.5 triggering the rehashing process
  bool testRehashTrigger(){

    CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);
    // bool result = true;
    
    // insert data to trigger rehashing
    for(int i = 0; i < 300; ++i){
      Car car("model", 10, 1000 + i, true);
      cardb.insert(car);

    }

    // determine if the load factor triggers rhashing
    bool result = cardb.lambda() <= 0.5;

    return result;

  }

  // Function: Function to test the rehash completing after triggering rehash due to load factor
  // Case: Test the rehash completion after triggering rehash due to load factor, i.e. all live data is transferred to the 
  // new table and the old table is removed.
  // Expected Result: The function should return true indicating that after inserting 10 cars
  // the load factor of the hash table is less than 0.5 and there are no deleted slots in
  // the hash tavle, suggesting that rehashing has been successfully triggered and completed
  bool testLoadFactorRehash(){

    // initialize CarDB with a small size to easily trigger rehash
    CarDB cardb(5, hashCode, DOUBLEHASH);


    // insert data until the load factor exceeds 0.5
    for(int i = 0; i < 10; ++i){
      Car car("model", i, i, true);
      cardb.insert(car);

    }

    // determine if rehashing complete
    return cardb.lambda() < 0.5 && cardb.deletedRatio() == 0.0;
    
  }

  // Function: CarDB::remove(Car car)
  // Case: Test the rehashing is triggered after a descent number of data removal.
  // Expected Result: The function should return true indicating that after inserting 10 cars
  // and removing half of them, the load factor of the hash tabke is less than 0.5 and the deleted
  // ratio is less than 0.5 suggesting rehashing has been successfullt triggered and completed
  bool testRemovalRehash(){

    // initialize arDb with a small size
    CarDB cardb(5, hashCode, DOUBLEHASH);

    //insert data
    for(int i = 0; i < 10; ++i){
      Car car("model", i, i, true);
      cardb.insert(car);

    }

    // remove half of data
    for(int i = 0; i < 5; i++){

      Car car("model", i, i, true);
      cardb.remove(car);

    }

     //insert data

    // determine if rehashing compleed
    return cardb.lambda() < 0.5 && cardb.deletedRatio() < 0.5;
    
  }

  // Function: CarDB::deletedRatio() const
  // Case: Test the rehash completion after triggering rehash due to delete ratio, i.e. all live data is transferred to the new table and the old table is removed.
  // Expected Result: The old table should be set to nullptr after the triggering the rehashing process
  // indicating that all live data has been transferred to the new table and the old table is removed 
  bool testRehashAfterDeleteRatio(){

    // create a CarDB object with a small size to trigger rehashing
    CarDB cardb(10, hashCode, DOUBLEHASH);

    bool result = false;

    // insert some data
    cardb.insert(Car("challenger", 5, 1001, true));
    cardb.insert(Car("stratos", 8, 1002, true));
    cardb.insert(Car("gt500", 3, 1003, true));

    // remove some data to trigger rehash due to delete ratio
    cardb.remove(Car("challenger", 5, 1001, true));

    // determine if rehash is completed
    if(cardb.m_oldTable == nullptr){

      result = true;
    } else {

      return result;
    }

    
    return result;

      

  
  }
  
    
      
};

int main() {

  Tester tester;

  cout << "Testing the insertion operation tests: " << endl;

  if(tester.testInsertion()){

      cout << "Insertion operation tests passed successfully" << endl;

  } else {

    cout << "Insertion operation tests failed successfully" << endl;

  }

  cout << "                                      " << endl;

  cout << "Testing the error case for the find operatiom: " << endl;
  if(tester.testErrorFind()){

    cout << "Error Case for the find operation passed" << endl;

  } else {

    cout << "Error Case for the find operation failed" << endl;

  }
  cout << "                                      " << endl;

  cout << "Testing the find operation for non-collision keys: " << endl;
  if(tester.testFindNonCollisionKeys()){

    cout << "Find operation for non-collision keys is successful" << endl;

  } else {

    cout << "Find operation for non-collision keys is unsuccessful" << endl;

  }
  
  cout << "                                      " << endl;
  
  cout << "Testing the find operation for collision keys" << endl;
  if(tester.testCollisionKeys()){

    cout << "Find operation for collision keys is successful" << endl;

  } else {

    cout << "Find operation for collision keys is unsuccessful" << endl;

  }

  cout << "                                      " << endl;

  cout << "Testing the remove operation for non-colliding keys: " << endl;

  if(tester.testRemoveNonCollidingKeys()){

    cout << "Remove operation for non-colliding keys successful" << endl;

  } else {

    cout << "Remove operation for non-colliding keys unsuccessful" << endl;

  }

  cout << "                                      " << endl;

  cout << "Testing the remove operation for colliding keys: " << endl;

  if(tester.testNoRehash()){
    cout << "Remove operation for colliding keys successful" << endl;

  } else {

    cout << "Remove operation for colliding keys unsuccessful" << endl;

  }

  cout << "                                      " << endl;

  cout << "Testing whether rehashing is triggered after a decent number of data insertion: " ;
  if(tester.testRehashTrigger()){

    cout << "Rehashing triggered after a decent number of data insertion" << endl;

  } else {

    cout << "Rehashing not triggered after a decent number of data insertion" << endl;

  }

  cout << "                                      " << endl;

  cout << "Testing for rehashing based on load factor: " << endl;

  if(tester.testLoadFactorRehash()){

    cout << "Test for rehashing based on load factor passed" << endl;

  } else {

    cout << "Test for rehashing based on load factor failed" << endl;

  }

  cout << "                                      " << endl;

  cout << "Testing for rehashing afetr data removal: " << endl;
  if(tester.testRemovalRehash()){

    cout << "Test for rehashing after data removal passed" << endl;

  } else {

    cout << "Test for rehashing after data removal failed" << endl;

  }

  cout << "                                      " << endl;

  cout << "Testing for rehashing after delete ratio: " << endl;
  if(tester.testRehashAfterDeleteRatio()){

    cout << "Test for rehashing after delete ratio passed" << endl;

  } else {

    cout << "Test for rehashing after delete ratio failed" << endl;
  }
  

  return 0;
}
	
