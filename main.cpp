#include <iostream>
#include <unordered_map>
#include <stdexcept>

using namespace std;

class InMemoryinmemoryDB {
private:
    unordered_map<string, int> data;
    unordered_map<string, int> transactionData;
    bool inTransaction = false;

public:
    void begin_transaction() {
        if (inTransaction) {
          //  throw runtime_error("Transaction already in progress.");
          cout << "I WILL THROW AN ERROR - TRANSACTION ALREADY IN PROGRESS" << endl;
          return;
        }
        inTransaction = true;
        cout << "Transaction in progress!" << endl;
    }

    void put(const string& key, int value) {
        if (!inTransaction) {
           // throw runtime_error("Transaction not in progress.");
           cout << "I WILL THROW AN ERROR - TRANSACTION NOT IN PROGRESS" << endl;
           return;
        }
        transactionData[key] = value;
        cout << "You initialized for: " << key << ", " << value << endl;
    }

    int get(const string& key) {
        if (data.find(key) != data.end()) {
            return data[key];
        }
        else if (data.find(key) == nullptr && transactionData.find(key) != nullptr){
            cout << key << " is not committed yet!" << endl;
            return 0;
        }
        else if (data.find(key) == nullptr)          {
            cout << key << " does not exist" << endl;
            return 0;
        }
        return 0; //  key doesn't exist
    }

    void commit() {
        if (!inTransaction) {
             // throw runtime_error("Transaction not in progress.");
             cout << "I WILL THROW AN ERROR - TRANSACTION NOT IN PROGRESS" << endl;
             return;
        }
        for (const auto& entry : transactionData) {
            data[entry.first] = entry.second;
        }
        transactionData.clear();
        cout << "Commit successful for all initialized, ending transaction." << endl;
        inTransaction = false;
    }

    void rollback() {
        if (!inTransaction) {
             // throw runtime_error("Transaction not in progress.");
             cout << "I WILL THROW AN ERROR - TRANSACTION NOT IN PROGRESS" << endl;
             return;
        }
        cout << "Reverting changes" << endl;
        transactionData.clear();
        inTransaction = false;
    }
};

int main() {
    InMemoryinmemoryDB inmemoryDB;

    // should return null, because A doesn’t exist in the DB yet
    string A = "A";
    string B = "B";
    inmemoryDB.get(A);

    // should throw an error because a transaction is not in progress
    inmemoryDB.put(A, 5);

    // starts a new transaction
    inmemoryDB.begin_transaction();

    // set’s value of A to 5, but its not committed yet
    inmemoryDB.put(A, 5);

    // should return null, because updates to A are not committed yet
    inmemoryDB.get(A);

    // update A’s value to 6 within the transaction
    inmemoryDB.put(A, 6);

    // commits the open transaction
    inmemoryDB.commit();

    // should return 6, that was the last value of A to be committed
    int returnA = inmemoryDB.get(A);
    cout << A << ": " << returnA << endl;

    // throws an error, because there is no open transaction
    inmemoryDB.commit();

    // throws an error because there is no ongoing transaction
    inmemoryDB.rollback();

    // should return null because B does not exist in the database
    inmemoryDB.get(B);

    // starts a new transaction
    inmemoryDB.begin_transaction();

    // Set key B’s value to 10 within the transaction
    inmemoryDB.put(B, 10);

    // Rollback the transaction - revert any changes made to B
    inmemoryDB.rollback();

    // Should return null because changes to B were rolled back
    inmemoryDB.get(B);

    return 0;
}
