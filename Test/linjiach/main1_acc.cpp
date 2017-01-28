#include <iostream>  
#include <string> 
#include <fstream>
#include <streambuf>
#include <vector>
#include <limits>
#include <cstring>
#include <algorithm>

#include "split.h"
// #include "Jason.h"
using namespace std;

class Jason{
private:
    int n_j;
    int mode_j;
    int score_j;
    int cputime_j;
    char youplay_j;
    char enemy_j;
    char** state_j;
    int** cell_j;
    int depth_j;
    int remaining_j;
    
    int min_n(char** s, int a, int b, int d, int r, int eval);
    int max_n(char** s, int a, int b, int d, int r, int eval);
    int calc(char** s, int i, int j, char yp, int & eval);
    // int utility(char** s);
    
public:
    Jason(string str);
    string decision(int d, int ret[]);
        
};

struct Node{
    int x = 0;
    int y = 0;
    int eval_t = 0;
    int raid = 0;
    // char** cur_state;
};

static bool comp_node(Node n1, Node n2){
    if(n1.eval_t>n2.eval_t) return true;
    return false;
}

Jason::Jason(string str){
    vector<string> inputdata;
    split_s(str,inputdata,'\n');
    n_j = stoi(inputdata[0]);
    // cout<<inputdata[0]<<endl;
    youplay_j = inputdata[2][0];
    cputime_j = stoi(inputdata[3]);
    if(youplay_j == 'X') enemy_j = 'O';
    else enemy_j = 'X';
    cell_j = new int*[n_j];
    for(int i = 0; i < n_j; i++){
        cell_j[i] = new int[n_j];
        for(int j = 0; j<n_j; j++){
            cell_j[i][j] = stoi(inputdata[i*n_j+4+j]);
            // cout<<cell_j[i][j]<<" ";
        }
        // cout<<endl;
    }
    state_j = new char*[n_j];
    int n_j_p = n_j*n_j;
    remaining_j = 0;
    score_j = 0;
    for(int i = 0; i < n_j; i++){
        state_j[i] = new char[n_j];
        for(int j = 0; j<n_j; j++){
            state_j[i][j] = inputdata[i+4+n_j_p][j];
            if(state_j[i][j] == '.') remaining_j +=1;
            if(state_j[i][j] == youplay_j) score_j +=cell_j[i][j];
            if(state_j[i][j] == enemy_j) score_j -=cell_j[i][j];
            // cout<<state_j[i][j]<<" ";
        }
        // cout<<endl;
    }
    // for(int i = 0; i < n_j; i++){
    //     for(int j = 0; j < n_j; j++){
    //         cout<<state_j[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }
    // if(inputdata[1])
}

string Jason::decision(int d, int ret[]){
    int a = numeric_limits<int>::min();
    int b = numeric_limits<int>::max();
    depth_j = d;
    if(remaining_j<=0 || depth_j==0) return "No move";
    int max_v = numeric_limits<int>::min();
    int copy_j = sizeof(char)*n_j*n_j;
    // char*** cur_state = (char ***)malloc(remaining_j*sizeof(char**));
    // for(int m = 0; m < remaining_j; m++){
    //     cur_state[m] = (char **)malloc(n_j*sizeof(char*));
    //     for(int k=0;k<n_j;k++){
    //         cur_state[m][k]=(char *) malloc(n_j);
    //         memcpy(cur_state[m][k],state_j[k], n_j);
    //     }
    // }
    vector<Node> nodes(remaining_j);
    // for(int m = 0; m < remaining_j; m++){
    //     nodes[m].cur_state = (char **)malloc(n_j*sizeof(char*));
    //     for(int k=0;k<n_j;k++){
    //         nodes[m].cur_state[k]=(char *) malloc(n_j);
    //         memcpy(nodes[m].cur_state[k],state_j[k], n_j);
    //     }
    // }
    // nodes[0].cur_state[0][0] = 'O';
    // for(int i1 = 0; i1 < n_j; i1++){
    //     for(int j1 = 0; j1 < n_j; j1++){
    //         cout<<nodes[0].cur_state[i1][j1]<<" ";
    //     }
    //     cout<<endl;
    // }
    // cout<<endl;
    // int* x = new int[remaining_j];
    // int* y = new int[remaining_j];
    // int* eval_t = new int[remaining_j];
    // for(int m = 0; m < remaining_j; m++) eval_t[m] = score_j;
    int index = 0;
    for(int i = 0; i < n_j; i++){
        for(int j = 0; j<n_j; j++){
            if(state_j[i][j] == '.'){
                char** cur_state = (char **)malloc(n_j*sizeof(char*));
                for(int k=0;k<n_j;k++){
                    cur_state[k]=(char *) malloc(n_j);
                    memcpy(cur_state[k],state_j[k], n_j);
                }
                // int size = sizeof(cur_state);
                nodes[index].eval_t+= score_j;
                nodes[index].raid = calc(cur_state,i,j, youplay_j, nodes[index].eval_t);
                nodes[index].x = i;
                nodes[index].y = j;
                for(int k = 0; k < n_j; k++)
                    delete cur_state[k];
                delete cur_state; 
                // for(int i1 = 0; i1 < n_j; i1++){
                //     for(int j1 = 0; j1 < n_j; j1++){
                //         cout<<nodes[index].cur_state[i1][j1]<<" ";
                //     }
                //     cout<<endl;
                // }
                // cout<<endl;
                index++;
            }
        }
    }
    sort(nodes.begin(),nodes.end(),comp_node);
    for(int i = 0; i < remaining_j; i++){
        char** cur_state = (char **)malloc(n_j*sizeof(char*));
        for(int k=0;k<n_j;k++){
            cur_state[k]=(char *) malloc(n_j);
            memcpy(cur_state[k],state_j[k], n_j);
        }
        int temp = 0;
        calc(cur_state,nodes[i].x,nodes[i].y, youplay_j, temp);
        int cur_val = min_n(cur_state, a, b, 1, remaining_j-1, -nodes[i].eval_t);
        for(int k = 0; k < n_j; k++)
            delete cur_state[k];
        delete cur_state;    
        if(cur_val > max_v){
            ret[0] = nodes[i].x;
            ret[1] = nodes[i].y;
            ret[2] = nodes[i].raid;
            max_v = cur_val;
        }
        if( max_v >= b){
            ret[0] = nodes[i].x;
            ret[1] = nodes[i].y;
            ret[2] = nodes[i].raid;
            string mat = "";
            calc(state_j, ret[0], ret[1], youplay_j, score_j);
            for(int ik = 0; ik < n_j-1; ik++){
                for(int jk = 0; jk<n_j; jk++){
                    mat.push_back(state_j[ik][jk]);
                }
                mat.push_back('\n');
            }
            for(int jk = 0; jk<n_j; jk++){
                mat.push_back(state_j[n_j-1][jk]);
            }
            return mat;
        }
        a = max(a,max_v);
    }
    cout<<"max"<<max_v<<endl;
    calc(state_j, ret[0], ret[1], youplay_j, score_j);
    string mat = "";
    for(int ik = 0; ik < n_j-1; ik++){
        for(int jk = 0; jk<n_j; jk++){
            mat.push_back(state_j[ik][jk]);
        }
        mat.push_back('\n');
    }
    for(int jk = 0; jk<n_j; jk++){
        mat.push_back(state_j[n_j-1][jk]);
    }
    return mat;
}

int Jason::max_n(char** s, int a, int b, int d, int r, int eval){
    // cout<<"max node in depth: "<<d<<endl;
    if(r<=0 || d==depth_j) return eval;
    int max_v = numeric_limits<int>::min();
    int copy_j = sizeof(char)*n_j*n_j;
    vector<Node> nodes(r);
    int index = 0;
    for(int i = 0; i < n_j; ++i){
        for(int j = 0; j<n_j; j++){
            if(s[i][j] == '.'){
                // for(int i1 = 0; i1 < n_j; i1++){
                //     for(int j1 = 0; j1 < n_j; j1++){
                //         cout<<nodes[index].cur_state[i1][j1]<<" ";
                //     }
                //     cout<<endl;
                // }
                char** cur_state = (char **)malloc(n_j*sizeof(char*));
                for(int k=0;k<n_j;k++){
                    cur_state[k]=(char *) malloc(n_j);
                    memcpy(cur_state[k],s[k], n_j);
                }
                nodes[index].eval_t+= eval;
                nodes[index].raid = calc(cur_state,i,j, youplay_j, nodes[index].eval_t);
                nodes[index].x = i;
                nodes[index].y = j;
                // for(int i1 = 0; i1 < n_j; i1++){
                //     for(int j1 = 0; j1 < n_j; j1++){
                //         cout<<cur_state[i1][j1]<<" ";
                //     }
                //     cout<<endl;
                // }
                for(int k = 0; k < n_j; k++)
                    delete cur_state[k];
                delete cur_state;  
                index++;
            }
        }
    }
    sort(nodes.begin(),nodes.end(),comp_node);
    for(int i = 0; i < r; i++){
        char** cur_state = (char **)malloc(n_j*sizeof(char*));
        for(int k=0;k<n_j;k++){
            cur_state[k]=(char *) malloc(n_j);
            memcpy(cur_state[k],s[k], n_j);
        }
        calc(cur_state,nodes[i].x,nodes[i].y, youplay_j, nodes[i].eval_t);
        max_v = max(min_n(cur_state, a, b, d+1, r-1, -nodes[i].eval_t), max_v);
        for(int k = 0; k < n_j; k++)
            delete cur_state[k];
        delete cur_state;   
        if( max_v >= b) return max_v;
        a = max(a,max_v);
    }
    return max_v;
}

int Jason::min_n(char** s, int a, int b, int d, int r, int eval){
    // cout<<"min node in depth: "<<d<<endl;
    if(r<=0 || d==depth_j) return -eval;
    int min_v = numeric_limits<int>::max();
    int copy_j = sizeof(char)*n_j*n_j;
    vector<Node> nodes(r);
    int index = 0;
    for(int i = 0; i < n_j; ++i){
        for(int j = 0; j<n_j; j++){
            if(s[i][j] == '.'){
                // for(int i1 = 0; i1 < n_j; i1++){
                //     for(int j1 = 0; j1 < n_j; j1++){
                //         cout<<nodes[index].cur_state[i1][j1]<<" ";
                //     }
                //     cout<<endl;
                // }
                // cout<<i<<" "<<j<<endl;
                char** cur_state = (char **)malloc(n_j*sizeof(char*));
                for(int k=0;k<n_j;k++){
                    cur_state[k]=(char *) malloc(n_j);
                    memcpy(cur_state[k],s[k], n_j);
                }
                nodes[index].eval_t+= eval;
                nodes[index].raid = calc(cur_state,i,j, enemy_j, nodes[index].eval_t);
                nodes[index].x = i;
                nodes[index].y = j;
                for(int k = 0; k < n_j; k++)
                    delete cur_state[k];
                delete cur_state;  
                index++;
            }
        }
    }
    sort(nodes.begin(),nodes.end(),comp_node);
    for(int i = 0; i < r; i++){
        char** cur_state = (char **)malloc(n_j*sizeof(char*));
        for(int k=0;k<n_j;k++){
            cur_state[k]=(char *) malloc(n_j);
            memcpy(cur_state[k],s[k], n_j);
        }
        calc(cur_state,nodes[i].x,nodes[i].y, enemy_j, nodes[i].eval_t);
        min_v = min(max_n(cur_state, a, b, d+1, r-1, -nodes[i].eval_t), min_v);
        for(int k = 0; k < n_j; k++)
            delete cur_state[k];
        delete cur_state;   
        if( min_v <= a) return min_v;
        b = min(b,min_v);

    }
    return min_v;
}

// int Jason::utility(char** s){
//     int ret = 0;
//     for(int i = 0; i < n_j; ++i){
//         for(int j = 0; j<n_j; j++){
//             if(s[i][j] == youplay_j) ret +=cell_j[i][j];
//             if(s[i][j] == enemy_j) ret -=cell_j[i][j];
//         }
//     }
//     return ret;
// }

int Jason::calc(char** s, int y, int x, char yp, int & eval){
    char ep = 'X';
    if( yp == 'X') ep = 'O';
    bool raid = false;
    bool raid_avail = false;
    if(x>0){
        if (s[y][x-1] == yp) raid_avail = true;
    }
    if(x<n_j-1 && !raid_avail){
        if (s[y][x+1] == yp) raid_avail = true;
    }
    if(y>0 && !raid_avail){
        if (s[y-1][x] == yp) raid_avail = true;
    }
    if(y<n_j-1 && !raid_avail){
        if (s[y+1][x] == yp) raid_avail = true;
    }
    if(raid_avail){
        if(x>0){
            if (s[y][x-1] == ep){
                s[y][x-1] = yp;
                eval += 2* cell_j[y][x-1];
                raid = true;
            }
        }
        if(x<n_j-1){
            if (s[y][x+1] == ep){
                s[y][x+1] = yp;
                eval += 2* cell_j[y][x+1];
                raid = true;
            }
        }
        if(y>0){
            if (s[y-1][x] == ep){
                s[y-1][x] = yp;
                eval += 2* cell_j[y-1][x];
                raid = true;
            }
        }
        if(y<n_j-1){
            if (s[y+1][x] == ep){
                s[y+1][x] = yp;
                eval += 2* cell_j[y+1][x];
                raid = true;
            }
        }
    }
    s[y][x] = yp;
    eval += cell_j[y][x];
    // cout<<endl;
    // for(int i = 0; i < n_j; ++i){
    //     for(int j = 0; j<n_j; j++){
    //         cout<<s[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }
    if(raid) return 1;
    else return 0;
}

int main(int argc, char** argv){  
    ifstream t("input.txt");
    string str((istreambuf_iterator<char>(t)),
                 istreambuf_iterator<char>());
    // cout<<str<<endl;
    t.close();
    Jason *agent = new Jason(str);
    int ret[3];
    int depth = 3;
    if(argc==2) depth = stoi(argv[1]);
    // cout<<depth<<endl;
    string mat = agent->decision(depth,ret);
    cout<<ret[0]<<" "<<ret[1]<<" "<<ret[2]<<endl;
    char col = 'A'+ret[1];
    string stake = "";
    if(ret[2] == 0){
        stake = "Stake";
    }else if(ret[2] == 1){
        stake = "Raid";
    }else{
        stake = "Invalid";
    }
    ofstream ot;
    ot.open("output.txt");
    if(ot.is_open()){
        ot<<col<<ret[0]+1<<" "<<stake<<endl;
        ot<<mat;
        ot.close();
        cout<<"able to";
    }else cout<<"unable to";

    // int n = stoi(inputlines[0]);
    // // int n = (char)'0';
    // cout<<n<<endl;
    return 0;  
}  