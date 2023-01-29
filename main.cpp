#include <iostream>
#include <utility>
#include <vector>

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
  return 0;
}