#include <iostream>
#include <utility>
#include <vector>
#include <unordered_set>

using namespace std;

const float FLOORBOUNUS = 1;
const float BUILDINGBOUNUS = 0.25;
const float FLOORPUNISH = FLOORBOUNUS;
const float BUILDINGPUNISH = 0;

class team{
public:
    unsigned int capacity;
    unordered_set<int> preferred;
    vector<int> tolerated;
    unordered_set<int> noway;
    int rank_score=0;

public:
    team() : capacity(0) {};
    team(unsigned int capacity, unordered_set<int> preferred, vector<int> tolerated, unordered_set<int> noway){
      this->capacity=capacity;
      this->preferred=std::move(preferred);
      this->tolerated=std::move(tolerated);
      this->noway=std::move(noway);
    }
    team(int teamnum, unsigned int capacity, unordered_set<int> preferred, vector<int> tolerated, int maxteamnumber){
      this->capacity=capacity;
      unordered_set<int> used(preferred.begin(), preferred.end());
      for (int i:tolerated) used.insert(i);
      used.insert(teamnum);
      this->preferred=std::move(preferred);
      this->tolerated=std::move(tolerated);
      unordered_set<int> nowayVec;
      for (int i=1; i<=maxteamnumber; i++) {
        if (used.find(i) == used.end()) nowayVec.insert(i);
      }
      this->noway = std::move(nowayVec);
    }
};

vector<int> rankTeam(vector<team> &allTeam){
  vector<int> out(allTeam.size(), 0);
  for (int i=0; i<allTeam.size(); i++){  // loop team
    for (int j : allTeam[i].preferred){  // loop team preference
      if ((allTeam[j-1].noway).find(i+1) == (allTeam[j-1].noway).end()) {// check for noway conflict
        out[j-1]++;
      }
    }
  }
  return out;
}

//(1,2,3)
float calScoreFloor(const vector<int>& floor, const vector<team> & teams){
  float out=0;
    for (int i : floor){
      for (int j : floor) {
        if (i!=j){
          if (teams[i-1].preferred.find(j) != teams[i-1].preferred.end()) out+=FLOORBOUNUS;
          else if (teams[i-1].noway.find(j) != teams[i-1].noway.end())  out-=FLOORPUNISH;
        }
      }
    }
  return out;
}

float calScoreBuilding(const vector<vector<int>>& groups, const vector<team> & teams){
  float out=0;
  for (int i=0; i<groups.size(); i++){  // target floor
    for(int j=0; j<groups.size(); j++){  // other floors
      if (i!=j) {
        for (int x : groups[i])  {  //target group on floors
          for (int y : groups[j]) {
            if (teams[y-1].preferred.find(x) != teams[y-1].preferred.end()) out += BUILDINGBOUNUS;
            if (teams[y-1].noway.find(x) != teams[y-1].noway.end()) out += BUILDINGPUNISH;
          }
        }
      }
    }
  }
  return out;
}

float calScorePlan(const vector<vector<int>>& groups, vector<team> & teams){
  float out = 0;
  for(const vector<int>& floor : groups) {
    float score = calScoreFloor(floor, teams);
    out += score;
  }
  out += calScoreBuilding(groups, teams);
  return out;
}

int main(){
  vector<int> floorCapacity = {43, 81, 73, 54, 97};
  vector<team> allTeam;
  allTeam.push_back(team(1, 22, {2,4,6,11}, {3,8,10}, 11));
  allTeam.push_back(team(2, 45, {1,3,5}, {6,7,11}, 11));
  allTeam.push_back(team(3, 34, {1,2,11}, {7}, 11));
  allTeam.push_back(team(4, 51, {10}, {1,3}, 11));
  allTeam.push_back(team(5, 11, {1,2,3,4}, {9,10,11}, 11));
  allTeam.push_back(team(6, 37, {7,10}, {1,8}, 11));
  allTeam.push_back(team(7, 42, {1,2,3,4,5,6}, {10,11}, 11));
  allTeam.push_back(team(8, 16, {1,10}, {2,4,11}, 11));
  allTeam.push_back(team(9, 29, {1,5}, {2,10}, 11));
  allTeam.push_back(team(10, 56, {2,6,7,11}, {4,5,8}, 11));
  allTeam.push_back(team(11, 49, {1,4,5}, {2,3,6,7,9,10}, 11));

  cout << calScorePlan({{1,2}, {3}}, allTeam) << endl;

  return 0;
}