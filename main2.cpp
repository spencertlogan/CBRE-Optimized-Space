#include <iostream>
#include <utility>
#include <vector>
#include <unordered_set>
#include<climits>

using namespace std;
int score;
class team{
public:
    unsigned int capacity;
    vector<int> preferred;
    vector<int> tolerated;
    unordered_set<int> noway;
    int rank_score=0;

public:
    team() : capacity(0) {};
    team(unsigned int capacity, vector<int> preferred, vector<int> tolerated, unordered_set<int> noway){
      this->capacity=capacity;
      this->preferred=std::move(preferred);
      this->tolerated=std::move(tolerated);
      this->noway=std::move(noway);
    }
    team(int teamnum, unsigned int capacity, vector<int> preferred, vector<int> tolerated, int maxteamnumber){
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

vector<vector<vector<int>>> all_floorplans;
int min_size=INT_MAX;
vector<int> og_floorCapacity;

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

void dfs(vector<team> allTeam, vector<int> floors, int floor_num, vector<vector<int>> floorplan, vector<bool> visited, int idx){
  
  //if no floor has space left for any team then add floor plan to the all floor plans
  bool flag = true;
  for(int f=0;f< floors.size(); f++){
    // cout<<"floor "<<f<<"capacity: "<< floors[f];
    if(floors[f]<0 || floors[f]>og_floorCapacity[f]/4){
      cout<<"floor capacity neg "<<endl;
      flag = false;
      break;
    }
    for(auto t : allTeam){
      if(t.capacity<=floors[f]){
        flag = false;
        cout<<"floor capacity exceeds team count"<<endl;
        break;
      }
    }
  }
    // cout<<endl;
  if(flag){
    cout<<"pushing floor plan"<<endl;
    all_floorplans.push_back(floorplan);
  }

  //check if valid recursion for floor plan
  if(visited[idx] || floors[floor_num]<0) return;
  for(int team_num : floorplan[floor_num]){
    if(allTeam[team_num - 1].noway.find(idx) != allTeam[team_num - 1].noway.end()) return;
  }

  if(floors[floor_num]<allTeam[idx].capacity) return;

  
    visited[idx] = true;
  for(int i =0;i<allTeam.size();i++){
    
    int team_size = allTeam[i].capacity;

    int team_num = i+1;
    cout<<"team: "<<team_num<<' ';
    for(int f=0;f<floors.size();f++){
      floors[f]-=team_size;
      // cout<<"****** floor "<< f<< "size "<<floors[f]<<endl;
      floorplan[f].push_back(team_num);
      // cout<<"floor: "<<f<<' ';
      dfs(allTeam, floors, f, floorplan, visited, i);
      floors[f]+=team_size;
    }
    cout<<endl;
    
  }
  // visited[i] = false;


}



int main(){
  vector<int> floorCapacity = {43, 81, 73, 54, 97};
  og_floorCapacity = floorCapacity;
  vector<team> allTeam;
  // allTeam.push_back(team(0, 0, {0}, {0}, 0));
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

  vector<vector<int>> floorplan(floorCapacity.size(), vector<int>());
  vector<bool> visited(allTeam.size(), false);
  dfs(allTeam, floorCapacity, 0, floorplan, visited, 0);
  cout<<"hello uhack"<<endl;
  for(auto fl : all_floorplans){
    cout<<"building"<<endl;
    for(auto building : fl){
      for(auto team_num : building){
        cout<<team_num<<' ';
      }
      cout<<endl;
    }
    cout<<endl;
  }
  
  cout<<"hello uhack 2"<<endl;
  // floor A
  // vector<int> rank 
  // for (team i : allTeam){

  //   for(int j: *i.change_noway()) cout << j << " ";
  //   cout << endl;
  // }
  return 0;
}