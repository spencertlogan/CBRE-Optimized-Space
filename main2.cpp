#include <iostream>
#include <utility>
#include <vector>
#include <set>

using namespace std;

class team{
  protected:
    unsigned int capacity;
    vector<int> preferred;
    vector<int> tolerated;
    vector<int> noway;

  public:
    team() : capacity(0) {};
    team(unsigned int capacity, vector<int> preferred, vector<int> tolerated, vector<int> noway){
      this->capacity=capacity;
      this->preferred=std::move(preferred);
      this->tolerated=std::move(tolerated);
      this->noway=std::move(noway);
    }
    team(int teamnum, unsigned int capacity, vector<int> preferred, vector<int> tolerated, int maxteamnumber){
      this->capacity=capacity;
      set<int> used(preferred.begin(), preferred.end());
      for (int i:tolerated) used.insert(i);
      used.insert(teamnum);
      this->preferred=std::move(preferred);
      this->tolerated=std::move(tolerated);
      vector<int> nowayVec;
      for (int i=1; i<=maxteamnumber; i++) {
        if (used.find(i) == used.end()) nowayVec.push_back(i);
      }
      this->noway = std::move(nowayVec);
    }
    vector<int> * change_prefrence(){
      return & preferred;
    }
    vector<int> * change_tolerated(){
      return & tolerated;
    }
    vector<int> * change_noway(){
      return & noway;
    }
};



int main(){
  vector<int> floorCapacity = {43, 81, 73, 54, 97};
  vector<team> allTeam;
  allTeam.push_back(team(1, 22, {2,4,6,11}, {3,8,10}, 11));
  allTeam.push_back(team(2, 45, {1,3,5}, {6,7,11}, 11));
  allTeam.push_back(team(3, 34, {2,4,6,11}, {3,8,10}, 11));
  allTeam.push_back(team(4, 51, {1,2,11}, {7}, 11));
  allTeam.push_back(team(5, 11, {10,1,3}, {1,3}, 11));
  allTeam.push_back(team(6, 37, {7,10}, {1,8}, 11));
  allTeam.push_back(team(7, 42, {1,2,3,4,5,6}, {10,11}, 11));
  allTeam.push_back(team(8, 16, {1,10}, {3,8,10}, 11));
  allTeam.push_back(team(9, 29, {1,5}, {3,8,10}, 11));
  allTeam.push_back(team(10, 56, {2,6,7,11}, {4,5,8}, 11));
  allTeam.push_back(team(11, 49, {1,4,5}, {2,3,6,7,9,10}, 11));

  cout<<"hello uhack"<<endl;
  // floor A
  for (team i : allTeam){
    for(int j: *i.change_noway()) cout << j << " ";
    cout << endl;
  }
  return 0;
}

