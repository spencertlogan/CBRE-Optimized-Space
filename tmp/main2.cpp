#include <iostream>
#include <utility>
#include <vector>
#include <unordered_set>
#include <string>
#include <sstream>

using namespace std;

class Team{
public:
  unsigned int capacity;
  unordered_set<int> preferred;
  vector<int> tolerated;
  unordered_set<int> noway;

public:
  Team() : capacity(0) {};
  Team(unsigned int capacity, unordered_set<int> preferred, vector<int> tolerated, unordered_set<int> noway){
    this->capacity=capacity;
    this->preferred=std::move(preferred);
    this->tolerated=std::move(tolerated);
    this->noway=std::move(noway);
  }
  Team(int teamnum, unsigned int capacity, vector<int> preferred, vector<int> tolerated, int maxteamnumber){
    this->capacity=capacity;
    unordered_set<int> used(preferred.begin(), preferred.end());
    for (int i:tolerated) used.insert(i);
    used.insert(teamnum);
    this->preferred=unordered_set<int>(preferred.begin(), preferred.end());
    this->tolerated=std::move(tolerated);
    unordered_set<int> nowayVec;
    for (int i=1; i<=maxteamnumber; i++) {
      if (used.find(i) == used.end()) nowayVec.insert(i);
    }
    this->noway = std::move(nowayVec);
  }
};

vector<vector<vector<int>>> all_floorplans;

vector<int> rankTeam(vector<Team> &allTeam){
  vector<int> out(allTeam.size(), 0);
  for (int i=0; i<allTeam.size(); i++){  // loop Team
    for (int j : allTeam[i].preferred){  // loop Team preference
      if ((allTeam[j-1].noway).find(i+1) == (allTeam[j-1].noway).end()) {// check for noway conflict
        out[j-1]++;
      }
    }
  }
  return out;
}

void dfs(vector<Team> allTeam, vector<int> floors, vector<vector<int>> floorplan, vector<bool> visited, vector<int> og_floorCapacity){
  // all_floorplans.push_back(floorplan);
  bool all_visited = true;
  for(auto vis : visited){
    if(!vis) all_visited = false;
  }
    //if no floor has space left for any Team then add floor plan to the all floor plans
  bool flag = true;
  for(int f=0;f< floors.size(); f++){
    // cout<<"floor "<<f<<"capacity: "<< floors[f];
    if(floors[f]<0 || floors[f]>og_floorCapacity[f]/4){
//       cout<<"floor capacity neg "<<endl;
      flag = false;
      break;
    }
    for(int t=0; t< allTeam.size();t++){
      if(allTeam[t].capacity<=floors[f] && !visited[t]){
        flag = false;
//         cout<<"floor capacity exceeds Team count"<<endl;
        break;
      }
    }
  }
  if(flag){
//     cout<<"********************************************************************pushing floor plan"<<endl;
    all_floorplans.push_back(floorplan);
  }
    if(all_visited) return;
  //check if valid recursion for floor plan
  // if( floors[floor_num]<0) return;
  // if(floors[floor_num]<allTeam[idx].capacity) return;

  for(int i =0;i<allTeam.size();i++){
    if(visited[i]) continue;
    visited[i] = true;

    int team_size = allTeam[i].capacity;

    int team_num = i+1;
    for(int f=0;f<floors.size();f++){
      bool noway = false;
      for(int team_n1 : floorplan[f]){
        if(allTeam[team_n1 - 1].noway.find(team_num) != allTeam[team_n1 - 1].noway.end()) noway=true;
      }
      if(noway) {
        continue;
      }
      if(floors[f]-team_size>=0){
        floors[f]-=team_size;
      } else{
        continue;
      }

      floorplan[f].push_back(team_num);
      dfs(allTeam, floors, floorplan, visited, og_floorCapacity);
      floorplan[f].pop_back();
      floors[f]+=team_size;
    }
  }
}

float calScoreFloor(const vector<int>& floor, const vector<Team> & teams, float floorbonus, float floorpunish){
  float out=0;
  for (int i : floor){
    for (int j : floor) {
      if (i!=j){
        if (teams[i-1].preferred.find(j) != teams[i-1].preferred.end()) out+=floorbonus;
        else if (teams[i-1].noway.find(j) != teams[i-1].noway.end())  out-=floorpunish;
      }
    }
  }
  return out;
}

float calScoreBuilding(const vector<vector<int>>& groups, const vector<Team> & teams, float buildingbonus, float buildingpunish){
  float out=0;
  for (int i=0; i<groups.size(); i++){  // target floor
    for(int j=0; j<groups.size(); j++){  // other floors
      if (i!=j) {
        for (int x : groups[i])  {  //target group on floors
          for (int y : groups[j]) {
            if (teams[y-1].preferred.find(x) != teams[y-1].preferred.end()) out += buildingbonus;
            if (teams[y-1].noway.find(x) != teams[y-1].noway.end()) out += buildingpunish;
          }
        }
      }
    }
  }
  return out;
}

float calScorePlan(const vector<vector<int>>& groups, vector<Team> & teams, float floorbonus, float floorpunish,
                   float buildingbonus, float buildingpunish){
  float out = 0;
  for(const vector<int>& floor : groups) {
    float score = calScoreFloor(floor, teams, floorbonus, floorpunish);
    out += score;
  }
  out += calScoreBuilding(groups, teams, buildingbonus, buildingpunish);
  return out;
}

vector<int> stringToVec(const string& rawstring){
  // helper to help convert string to int vector
  vector<int> out;
  stringstream ss(rawstring);
  string capacity;
  while (!ss.eof()) {
    getline(ss, capacity, ',');
    out.push_back(stoi(capacity));
  }
  return out;
}

string printVector(const vector<int>& raw){
  string out;
  for (int i:raw) out += to_string(i) + ",";
  return out;
}

class Driver{
  protected:
    vector<int> floorCapacity;
    vector<int> og_floorCapacity;
    vector<Team> allTeam;
    float floorBonus = 1;
    float buildingBonus = 0;
    float floorPunish = floorBonus;
    float buildingPunish = 0;
  public:
    void setFloorCapacity(const string& floordata){
      clearFloorCapacity();
      floorCapacity = stringToVec(floordata);
      og_floorCapacity = floorCapacity;
    }
    void clearFloorCapacity(){
      floorCapacity={};
      og_floorCapacity={};
    }
    void addTeam(int teamNum, int capacity, const string& preferred, const string& tolerated, int maxteamnumber){
      vector<int> preferredVec = stringToVec(preferred);
      vector<int> toleratedVec = stringToVec(tolerated);
      allTeam.emplace_back(teamNum, capacity, preferredVec, toleratedVec, maxteamnumber);
    }
    void removeTeam(int teamNum) {allTeam.erase(allTeam.begin() + teamNum-1);}
    void clearTeams() {allTeam.clear();}
    void debugTeam(){
      for (int i=0; i<allTeam.size(); i++){
        cout << "Team: " << i+1 <<",capacity: "<<allTeam[i].capacity << ", preferred: " <<
          printVector(vector<int>(allTeam[i].preferred.begin(), allTeam[i].preferred.end()))
          << " tolerate: " << printVector(allTeam[i].tolerated) << " noway: " <<
          printVector(vector<int>(allTeam[i].noway.begin(), allTeam[i].noway.end()))<< endl;
      }
    }
    
    void setFloorBonus(float i) {floorBonus=i;}
    void setFloorPunish(float i) {floorPunish=i;}
    void setBuildingBonus(float i) {buildingBonus=i;}
    void setBuildingPunish(float i) {buildingPunish=i;}
    
    string getBestLayout(){
      vector<vector<int>> floorplan(floorCapacity.size(), vector<int>());
      vector<bool> visited(allTeam.size(), false);
      dfs(allTeam, floorCapacity, floorplan, visited, og_floorCapacity);

      vector<vector<int>> result;
      float max_score = 0;
      for(const auto& fl : all_floorplans){
        float score = calScorePlan(fl, allTeam, floorBonus, floorPunish,
                                   buildingBonus, buildingPunish);
        if(score > max_score){
          result = fl;
          max_score = score;
        }
      }
      string out;
      for(int flr = 0; flr<result.size(); flr++){
        out += "Floor " + to_string(flr+1) + " : ";
        for(int tm : result[flr]){
          out += to_string(tm) + ' ';
        }
        out += "\n";
      }
      out += "Design rating: " + to_string(max_score);
      return out;
    }
};

int main() {
  Driver driver;
  // floor capacity
  int floorNum;
  cin >> floorNum;
  string d1, d2;
  for (int i=0; i< floorNum; i++) {
    cin >> d1;
    d2+=d1+',';
  }
  d2.pop_back();
  driver.setFloorCapacity(d2);

  // team number
  int teamNum;
  cin >> teamNum;
  for (int i=0; i<teamNum; i++){ //team
    //strength
    int str; cin >> str;
    //prefer & tolerate
    string prefer, tolerate;
    int i1; cin >> i1;
    for (int j=0; j< i1; j++) {
      cin >> d1;
      prefer+=d1+',';
    }
    prefer.pop_back();

    cin >> i1;
    for (int j=0; j< i1; j++) {
      cin >> d1;
      tolerate+=d1+',';
    }
    tolerate.pop_back();
    driver.addTeam(i+1, str, prefer, tolerate, teamNum);
  }

  cout << driver.getBestLayout()<<endl;
  return 0;
}
//int main() {
//  Driver driver;
//  bool run = true;
//  while (run) {
//    int option;
//    cout << "choose option:\n"
//            "1) Set Floor Capacity\n"
//            "2) Add Team\n"
//            "3) Remove Team Number\n"
//            "4) Clear Teams\n"
//            "5) Calculate Best Layout\n"
//            "6) Print All Teams\n"
//            "7) stop\n";
//    cin >> option;
//    if (option == 1) {
//      cout << "Give me floor capacity (use comma for splitting): ";
//      string raw;
//      cin >> raw;
//      driver.setFloorCapacity(raw);
//    } else if (option == 2) {
//      cout << "Add team information (teamNum, capacity, preferred, tolerated, max number of team): ";
//      int tn, c, mtn;
//      string r1, r2;
//      cin >> tn; cin >> c; cin >> r1, cin >> r2, cin >> mtn;
//      driver.addTeam(tn, c, r1, r2, mtn);
//    } else if(option == 3){
//      cout << "which team: ";
//      int i; cin >> i;
//      driver.removeTeam(i);
//    } else if(option == 4){
//      cout << "All teams are clear\n";
//      driver.clearTeams();
//    } else if(option == 5) {
//      cout << driver.getBestLayout();
//    } else if(option == 6){
//      driver.debugTeam();
//    } else if (option == 7){
//      run = false;
//    }
//  }
//  return 0;
//}