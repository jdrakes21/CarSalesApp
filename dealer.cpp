/*****************************************
** File:    dealer.cpp
** Project: CMSC 341 Project 4, Fall 2023
** Author:  Jervon Drakes
** Date:    12/05/2023
** E-mail:  jdrakes1@umbc.edu
** 
**
** This file contains the dealer.cpp file for Project 4.
** This program reads the dealer.h file and its functions and
** executes those functions for implementation
**
**
**
***********************************************/



// CMSC 341 - Fall 2023 - Project 4
#include "dealer.h"

// CarDB(constructor)
//The constructor takes size to specify the length of the current hash table, and hash is a function pointer to a hash function. The type of hash is defined in CarDB.h.
//The table size must be a prime number between MINPRIME and MAXPRIME. If the user passes a size less than MINPRIME, the capacity must be set to MINPRIME. If the user passes a size larger than MAXPRIME, the capacity must be set to MAXPRIME. If the user passes a non-prime number the capacity must be set to the smallest prime number greater than user's value. The probing parameter specifies the type of collision handling policy for the current hash table.
//Moreover, the constructor creates memory for the current table and initializes all member variables.
CarDB::CarDB(int size, hash_fn hash, prob_t probing = DEFPOLCY){

  m_hash = hash;

  // ensure that the size is within the valid range
  if(size < MINPRIME){

    m_currentCap = MINPRIME;

      
  } else if (size > MAXPRIME) {

    m_currentCap = MAXPRIME;

    
  } else {

    // find the smallest prime numer grater than or equal to the given size
    m_currentCap = findNextPrime(size);
    
  }

  // allocate memory for the current table
  m_currentTable = new Car[m_currentCap];

  // initalize member variables
  m_currentSize = 0;
  m_currentCap = size;
  m_currNumDeleted = 0;
  m_currProbing = probing;
  m_oldTable = nullptr;
  m_oldCap = 0;
  m_oldSize = 0;
  m_oldNumDeleted = 0;
  m_oldProbing = DEFPOLCY;

  
  //default collision handling policy for the new table
  m_newPolicy = DEFPOLCY;
  

  for(int i = 0; i < m_currentCap; i++){

    m_currentTable[i] = EMPTY;

  }
}


// ~CarDB(destructor)
//   Destructor deallocates the memory.
CarDB::~CarDB(){

  // deallocate memory for the current table
  delete[] m_currentTable;

  // deallocate memory for the old table if exists
  if(m_oldTable != nullptr){

    delete[] m_oldTable;

  }
  

}

// changeProbPolicy
// The user can change the collision handling policy of the hash table at the runtime. If a change request has been submitted by a user, the new policy will be stored in the variable m_newPolicy. Once the next rehash operation is initiated the new policy 
// will be used for the newly created hash table.
void CarDB::changeProbPolicy(prob_t policy){

  m_newPolicy = policy; // set the new collision handling policy
}


// insert
// This function inserts an object into the current hash table. The insertion index is determined by applying the hash function m_hash that is set in the CarDB constructor and then reducing the output of the hash function modulo the table size. A hash function is provided in the sample driver.cpp file to be used in this project.
//Hash collisions should be resolved using the probing policy specified in the m_currProbing variable. We insert into the table indicated by m_currentTable. After every insertion we need to check for the proper criteria, and if it is required, we need to rehash the entire table incrementally into a new table. The incremental transfer proceeds with 25% of the nodes at a time. Once we transferred 25% of the nodes for the first time, the second 25% will be transferred at the next operation (insertion or removal). Once all data is transferred to the new table, the old table will be removed, and its memory will be deallocated.
//If the Car object is inserted, the function returns true, otherwise it returns false. A Car object can only be inserted once. The hash table does not contain duplicate objects. Moreover, the car's ID value should be a valid one falling in the range [MINID-MAXID]. Every Car object is a unique object carrying the Car's model and the dealer's ID. The Car's model is the key which is used for hashing.
bool CarDB::insert(Car car) {
  // Check if the load factor exceeds 0.5 and rehash if necessary
  if (lambda() > 0.5) {

    // calculate new table size and create a new table 
    int newSize = findNextPrime(4 * (m_currentSize - m_currNumDeleted));
    Car* newTable = new Car[newSize];

    // move the current table to the old table 
    delete[] m_oldTable;
    m_oldTable = m_currentTable;
    m_oldCap = m_currentCap;
    m_oldSize = m_currentSize;
    m_oldNumDeleted = m_currNumDeleted;
    m_oldProbing = m_currProbing;
    
    // set the new table as the current table
    m_currentTable = newTable;
    m_currentCap = newSize;
    m_currentSize = 0;
    m_currNumDeleted = 0;
    m_currProbing = m_newPolicy;

    // Transfer 25% of the data from the old table to the new table
    int transferSize = floor(0.25 * m_oldSize);
    int transferred = 0;
    for (int i = 0; i < m_oldCap && transferred < transferSize; i++) {
      if (m_oldTable[i].getUsed() && m_oldTable[i].getModel() != "") {
	insert(m_oldTable[i]);
	m_oldTable[i].setUsed(false);
	m_oldNumDeleted--;
	transferred++;
      }
    }
  }

  // Find the insertion index using the hashing function and probing policy
  int index = m_hash(car.getModel()) % m_currentCap;
  int i = 0;
  int jumpSize = 0;

  while (m_currentTable[index].getUsed() && m_currentTable[index].getModel() != "") {

    if (m_currentTable[index] == car) {

      return false;  // Object already exists in the hash table

    }
    
    // calculate the jump size based on the probing policy
    if (m_currProbing == QUADRATIC) {

      jumpSize = i * i;
    }
    else if (m_currProbing == DOUBLEHASH) {

      jumpSize = i * (11 - (m_hash(car.getModel()) % 11));
    }

    // update the indez using the jump size and probing policy
    index = (index + jumpSize) % m_currentCap;
    i++;
  }

  // Insert the car object at the determined index
  m_currentTable[index] = car;
  m_currentTable[index].setUsed(true);
  m_currentSize++;
  
  return true;
}

// remove
// This function removes a data point from either the current hash table or the old hash table where the object is stored. In a hash table we do not empty the bucket, we only tag it as deleted. To tag a removed bucket we can use the member variable m_used in the Car class. To find the bucket of the object we should use the proper probing policy for the table.
//After every deletion we need to check for the proper criteria, and if it is required, we need to rehash the entire table incrementally into the current table. The incremental transfer proceeds with 25% of the nodes at a time. Once we transferred 25% of the nodes for the first time, the second 25% will be transferred at the next operation (insertion or removal). Once all data is transferred to the new table, the old table will be removed, and its memory will be deallocated.
//If the Car object is found and is deleted, the function returns true, otherwise it returns false.
bool CarDB::remove(Car car) {
  
  Car* oldTable = (m_oldProbing == NONE) ? nullptr : m_oldTable;

  // Use the proper probing policy to find the index
  int probeIndex = -1;
  if (m_currProbing != NONE) {
    int i = 0;

    while (i < m_currentCap) {
      probeIndex = ((m_hash(car.getModel()) % m_currentCap) + (i * i)) % m_currentCap;
      if (!m_currentTable[probeIndex].getUsed() || m_currentTable[probeIndex] == car) {
	break;
      }
      i++;
    }
  }

  // If the Car is found in the current table, mark it as deleted
  if (probeIndex != -1 && m_currentTable[probeIndex] == car) {
    m_currentTable[probeIndex].setUsed(false);
    m_currNumDeleted++;

    // Check if rehash is needed
    if ((float)m_currNumDeleted / (float)(m_currentCap - m_currNumDeleted) > 0.8 || lambda() > 0.5) {

      // Rehash incrementally
      int countTransfer = m_oldSize / 4;

      while (countTransfer > 0) {
	for (int i = 0; i < m_oldCap; i++) {
	  if (oldTable[i].getUsed()) {

	    insert(oldTable[i]);
	    oldTable[i].setUsed(false);
	    countTransfer--;

	  }
	}
      }

      // Deallocate memory for the old table
      delete[] m_oldTable;
      m_oldTable = nullptr;
      m_oldCap = 0;
      m_oldSize = 0;
      m_oldNumDeleted = 0;
    }
    
    return true;
  }

  // If probing policy for old table is different, search in old table as well
  if (m_oldProbing != m_currProbing) {

    int indexOld = -1;
    if (m_oldProbing != NONE) {

      int i = 0;
      while (i < m_oldCap) {

	indexOld = ((m_hash(car.getModel()) % m_oldCap) + (i * i)) % m_oldCap;
	if (oldTable[indexOld].getUsed() && oldTable[indexOld] == car) {

	  break;
	}
	i++;
      }
    }

    // If the Car is found in the old table, mark it as deleted
    if (indexOld != -1 && oldTable[indexOld] == car) {

      oldTable[indexOld].setUsed(false);
      m_oldNumDeleted++;

      // Check if rehash is needed for the old table
      if ((float)m_oldNumDeleted / (float)(m_oldCap - m_oldNumDeleted) > 0.8) {
	// Deallocate memory for the old table
	delete[] m_oldTable;
	m_oldTable = nullptr;
	m_oldCap = 0;
	m_oldSize = 0;
	m_oldNumDeleted = 0;
      }
      
      return true;
    }
  }
  
  // Car not found
  return false;
}

// getCar
// This function looks for the Car object with the model and the dealer id in the hash table, if the object is found the function returns it, otherwise the function returns empty object. If there are 
// two hash tables at the time, the function needs to search both tables.
Car CarDB::getCar(string model, int dealer) const{

  // check if the current table has data
  if(m_currentSize > 0){
    int i = 0;
    int probingIndex;

    // search in the current table based on the current probing policy (quadratic)
    if(m_currProbing == QUADRATIC) {

      while( i < m_currentCap) {

	probingIndex = (m_hash(model) % m_currentCap + i * i) % m_currentCap;

	if(m_currentTable[probingIndex].getModel() == model && m_currentTable[probingIndex].getDealer() == dealer && m_currentTable[probingIndex].getUsed()) {

	  return m_currentTable[probingIndex];

	}
	
	i++;

      }
    } else if(m_currProbing == DOUBLEHASH) {

      // search in the current table based on the current probing policy (Double Hash)
      while(i < m_currentCap){

	probingIndex = (m_hash(model) % m_currentCap + i * ( 11 - (m_hash(model) % 11))) % m_currentCap;

	if(m_currentTable[probingIndex].getModel() == model && m_currentTable[probingIndex].getDealer() == dealer && m_currentTable[probingIndex].getUsed()){

	  return m_currentTable[probingIndex]; // return the found car

	}
	i++;

      }
    }
  }

  // determine if the old table has data
  if(m_oldSize > 0){

    int i = 0;
    int index;
    
    // search in the old table based on the old probing policy
    if(m_oldProbing == QUADRATIC){

      while( i < m_oldCap){

	index = (m_hash(model) % m_oldCap + i * i) % m_oldCap;
	if(m_oldTable[index].getModel() == model && m_oldTable[index].getDealer() == dealer && m_oldTable[index].getUsed()){

	  // return the found car
	  return m_oldTable[index]; 

	}
	i++;
      }

    } else if(m_oldProbing == DOUBLEHASH){
      
      // search in the old table based on the old probing policy(double hash)
      while( i < m_oldCap){

	index = (m_hash(model) % m_oldCap + i * (11 - (m_hash(model) % 11))) % m_oldCap;

	if(m_oldTable[index].getModel() == model && m_oldTable[index].getDealer() == dealer && m_oldTable[index].getUsed()) {

	  return m_oldTable[index];

	}
	i++;

      }
    }
  }
  
  // if not found in either table return an empty Car ibject
  return EMPTY;
}

// lambda
// This function returns the load factor of the current hash table. The load factor is the ratio of occupied buckets to the table capacity. The number of occupied buckets is the total of available buckets and deleted buckets. An occupied bucket is a bucket which can 
// contain either a live data node (available to be used) or a deleted node.
float CarDB::lambda() const {

  // calculate the load factor
  int totalOccupied = m_currentSize + m_currNumDeleted; 
  return static_cast<float>(totalOccupied) / static_cast<float>(m_currentCap);
}

// deletedRatio
// This function returns the ratio of the deleted buckets to the total number of occupied buckets .
float CarDB::deletedRatio() const {

  // calculate the ratio of deleted cars to total occupied slots
  int occupiedTotal = m_currentSize - m_currNumDeleted;

  // check for division by zero
  if(occupiedTotal == 0){

    return 0.0;

  }

  
  float carRatio = static_cast<float>(m_currNumDeleted) / static_cast<float>(occupiedTotal);

  return carRatio;
    
}

void CarDB::dump() const {
    cout << "Dump for the current table: " << endl;
    if (m_currentTable != nullptr)
        for (int i = 0; i < m_currentCap; i++) {
            cout << "[" << i << "] : " << m_currentTable[i] << endl;
        }
    cout << "Dump for the old table: " << endl;
    if (m_oldTable != nullptr)
        for (int i = 0; i < m_oldCap; i++) {
            cout << "[" << i << "] : " << m_oldTable[i] << endl;
        }
}

// updateQuantity
// This function looks for the Car object in the hash table, if the object is found the function updates its quantity and returns true, otherwise the function returns false. If there are two hash tables at the time, the 
// function needs to search both tables for the search operation.
bool CarDB::updateQuantity(Car car, int quantity){

  bool value = false;
  
  // search in the current table
  int hashCurrent = m_hash(car.getModel()) % m_currentCap;
  int i = 0;
  int indexCurrent = hashCurrent;

  // iterate through the current table using quadratic probing
  while(m_currentTable[indexCurrent].getUsed() && i < m_currentCap){

    if(m_currentTable[indexCurrent] == car){

      //car found in the current table
      m_currentTable[indexCurrent].setQuantity(quantity);
      value = true;

    }

    i++;
    indexCurrent = (hashCurrent + i * i) % m_currentCap; // quadratic probing

  }

  // if there is an old table, search in table
  if(m_oldTable != nullptr){

    int hashOld = m_hash(car.getModel()) % m_oldCap;
    i = 0;
    int indexOld = hashOld;

    // iterate through the old table using double hash probing
    while(m_oldTable[indexOld].getUsed() && i < m_oldCap){
      
      if(m_oldTable[indexOld] == car){

	// car found in old table
	m_oldTable[indexOld].setQuantity(quantity);
	value = true;

      }
      i++;
      indexOld = (hashOld + i * (11 - ( hashOld & 11))) % m_oldCap; // double has probing

    }

  }

  return value;
}

// isPrime 
//This function dumps the contents of the current hash table and the old hash table if it exists. It prints the contents 
// of the hash table in array-index order 
bool CarDB::isPrime(int number){
    bool result = true;
    for (int i = 2; i <= number / 2; ++i) {
        if (number % i == 0) {
            result = false;
            break;
        }
    }
    return result;
}

// findNextPrime 
// This function returns the smallest prime number greater than the argument "current". If "current" is less than or equal to MINPRIME, the function returns MINPRIME. If "current" is greater than or equal to MAXPRIME, the function returns MAXPRIME. In a hash table we'd like to use a table with prime size. 
// Then, every time we need to determine the size for a new table, we use this function.
int CarDB::findNextPrime(int current){
    //we always stay within the range [MINPRIME-MAXPRIME]
    //the smallest prime starts at MINPRIME
    if (current < MINPRIME) current = MINPRIME-1;
    for (int i=current; i<MAXPRIME; i++) { 
        for (int j=2; j*j<=i; j++) {
            if (i % j == 0) 
                break;
            else if (j+1 > sqrt(i) && i != current) {
                return i;
            }
        }
    }
    //if a user tries to go over MAXPRIME
    return MAXPRIME;
}

ostream& operator<<(ostream& sout, const Car &car ) {
    if (!car.m_model.empty())
        sout << car.m_model << " (" << car.m_dealer << "," << car.m_quantity<< ")";
    else
        sout << "";
  return sout;
}

bool operator==(const Car& lhs, const Car& rhs){
    // since the uniqueness of an object is defined by model and delaer
    // the equality operator considers only those two criteria
    return ((lhs.m_model == rhs.m_model) && (lhs.m_dealer == rhs.m_dealer));
}
