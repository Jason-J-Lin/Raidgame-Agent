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
    int n_j_p;
    int mode_j;
    float cputime_j;
    char youplay_j;
    char enemy_j;
    char** state_j;
    int** cell_j;
    int depth_j;
    int remaining_j;
    int x_j;
    int y_j;
    bool large_set;
    int largest_j;
    int score_j;
    vector<int> calibrate;
    int max_d;
    
    int raidAward(char** s, int x, int y, char target);
    bool instantRaid(char** s, int i, int j, int ret[], bool defend);
    int min_n(char** s, int a, int b, int d, int r, int eval);
    int max_n(char** s, int a, int b, int d, int r, int eval);
    int calc(char** s, int i, int j, char yp, int & eval);
    int utility(char** s);
    
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
    split_s(str,inputdata,'\n');  //be careful here is a max split 1024
    n_j = stoi(inputdata[0]);
    if(n_j>5) large_set = true;
    else large_set = false;
    // cout<<inputdata[0]<<endl;
    youplay_j = inputdata[2][0];
    cputime_j = stof(inputdata[3]);
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
    n_j_p = n_j*n_j;
    remaining_j = 0;
    largest_j = 0;
    for(int i = 0; i < n_j; i++){
        state_j[i] = new char[n_j];
        for(int j = 0; j<n_j; j++){
            state_j[i][j] = inputdata[i+4+n_j_p][j];
            if(state_j[i][j] == '.'){
                remaining_j +=1;
                largest_j = max(cell_j[i][j], largest_j);
            }
            // cout<<state_j[i][j]<<" ";
        }
        // cout<<endl;
    }
    string cali;
    ifstream cal("cal1.txt");
    bool hasline = false;
    if(cal.good()){
        for(int i = 0; i < n_j-4; i++) cal.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        if(getline(cal,cali)) hasline = true;
    }
    if(hasline){
        split_i(cali,calibrate,' ');
        max_d = calibrate.size(); 
    }else{
        if(n_j>16) max_d = 3;
        else if(n_j>9) max_d = 4;
        else if(n_j>5) max_d = 6;
        else max_d = 8;
    }

    
    
    // for(int i = 0; i < n_j; i++){
    //     for(int j = 0; j < n_j; j++){
    //         cout<<state_j[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }
    // if(inputdata[1])
}

int Jason::raidAward(char** s1, int x, int y, char target){
    int ret_ra = 0;
    ret_ra += cell_j[x][y];
    if(x<n_j-1) if(s1[x+1][y] == target) ret_ra += cell_j[x+1][y];
    if(x>0) if(s1[x-1][y] == target) ret_ra += cell_j[x-1][y];
    if(y<n_j-1) if(s1[x][y+1] == target) ret_ra += cell_j[x][y+1];
    if(y>0) if(s1[x][y-1] == target) ret_ra += cell_j[x][y-1];
    return ret_ra;
}

bool Jason::instantRaid(char** s, int i, int j, int save[], bool defend){
    char target = enemy_j;
    bool def = false;
    if(defend) target = youplay_j;
    if(i>1){
        if(s[i-2][j] == target){
            if(raidAward(s, i-1, j, target) > save[0] && s[i-1][j] == '.'){
                save[0] = raidAward(s, i-1, j, target);
                save[1] = i-1;
                save[2] = j;
                def = defend;
            }
        }
    }
    if(i<n_j-2){
        if(s[i+2][j] == target){
            if(raidAward(s, i+1, j, target) > save[0] && s[i+1][j] == '.'){
                save[0] = raidAward(s, i+1, j, target);
                save[1] = i+1;
                save[2] = j;
                def = defend;
            }
        }
    }
    if(j>1){
        if(s[i][j-2] == target){
            if(raidAward(s, i, j-1, target) > save[0] && s[i][j-1] == '.'){
                save[0] = raidAward(s, i, j-1, target);
                save[1] = i;
                save[2] = j-1;
                def = defend;
            }
        }
    }
    if(j<n_j-2){
        if(s[i][j+2] == target){
            if(raidAward(s, i, j+1, target) > save[0] && s[i][j+1] == '.'){
                save[0] = raidAward(s, i, j+1, target);
                save[1] = i;
                save[2] = j+1;
                def = defend;
            }
        }
    }
    //  ----------------------------------------
    if(j>0 && i>0){
        if(s[i-1][j-1] == target){
            if(raidAward(s, i, j-1, target) > save[0] && s[i][j-1] == '.'){
                save[0] = raidAward(s, i, j-1, target);
                save[1] = i;
                save[2] = j-1;
                def = defend;
            }else if(raidAward(s, i-1, j, target) > save[0] && s[i-1][j] == '.'){
                save[0] = raidAward(s, i-1, j, target);
                save[1] = i-1;
                save[2] = j;
                def = defend;
            }
        }
    }
    if(j<n_j-1 && i>0){
        if(s[i-1][j+1] == target){
            if(raidAward(s, i, j+1, target) > save[0] && s[i][j+1] == '.'){
                save[0] = raidAward(s, i, j+1, target);
                save[1] = i;
                save[2] = j+1;
                def = defend;
            }else if(raidAward(s, i-1, j, target) > save[0] && s[i-1][j] == '.'){
                save[0] = raidAward(s, i-1, j, target);
                save[1] = i-1;
                save[2] = j;
                def = defend;
            }
        }
    }
    if(j>0 && i <n_j-1){
        if(s[i+1][j-1] == target){
            if(raidAward(s, i, j-1, target) > save[0] && s[i][j-1] == '.'){
                save[0] = raidAward(s, i, j-1, target);
                save[1] = i;
                save[2] = j-1;
                def = defend;
            }else if(raidAward(s, i+1, j, target) > save[0] && s[i+1][j] == '.'){
                save[0] = raidAward(s, i+1, j, target);
                save[1] = i+1;
                save[2] = j;
                def = defend;
            }
        }
    }
    if(j<n_j-1 && i <n_j-1){
        if(s[i+1][j+1] == target){
            if(raidAward(s, i, j+1, target) > save[0] && s[i][j+1] == '.'){
                save[0] = raidAward(s, i, j+1, target);
                save[1] = i;
                save[2] = j+1;
                def = defend;
            }else if(raidAward(s, i+1, j, target) > save[0] && s[i+1][j] == '.'){
                save[0] = raidAward(s, i+1, j, target);
                save[1] = i+1;
                save[2] = j;
                def = defend;
            }
        }
    }
}

string Jason::decision(int d, int ret[]){
    int a = numeric_limits<int>::min();
    int b = numeric_limits<int>::max();
    // depth_j = d;
    if(cputime_j < 50) max_d--;
    if(remaining_j<10 && cputime_j>=5){
        mode_j = 4;
        depth_j = 9;
    }else if(remaining_j<28 && cputime_j>100){
        mode_j = 4;
        depth_j = max(max_d,9);
    }else if(remaining_j<25 && cputime_j>=50){
        mode_j = 3;
        depth_j = max(max_d,8);
    }else if(remaining_j<15 && cputime_j>70){
        mode_j = 4;
        depth_j = max(max_d,9);
    }else if(remaining_j<15 && cputime_j>=20){
        mode_j = 3;
        depth_j = max(max_d,8);
    }else if(remaining_j<15 && cputime_j>=10){
        mode_j = 3;
        depth_j = max(max_d,6);
    }else if(cputime_j <30){
        cout<<"<30 !!! out of time!!!!"<<endl;
        depth_j = max(2,max_d-2);
        if(remaining_j <10 && cputime_j >5) depth_j = 8;
    }else{
        if(remaining_j > n_j_p*3/5){
            //  -------------------start instant raid---------------------
            // int raid = 0;
            // int raidx = 0;
            // int raidy = 0;
            bool defend = false;
            int save[3];
            save[0] = 0;
            for(int i = 0; i < n_j; i++){
                for(int j = 0; j<n_j; j++){
                    if(state_j[i][j] == youplay_j){
                        //  ----------------------------------------
                        defend = Jason::instantRaid(state_j, i, j, save, false);
                        //  ----------------------------------------
                    }else if(state_j[i][j] == enemy_j){
                        defend = Jason::instantRaid(state_j, i, j, save, true);
                    }
                }
            }
            if(save[0]>0){
                ret[0] = save[1];
                ret[1] = save[2];
                ret[2] = 1;
                string mat = "";
                calc(state_j, ret[0], ret[1], youplay_j, score_j);
                for(int ik = 0; ik < n_j; ik++){
                    for(int jk = 0; jk<n_j; jk++){
                        mat.push_back(state_j[ik][jk]);
                    }
                    mat.push_back('\n');
                }
                if(defend) cout<<"instant defend"<<endl;
                else cout<<"instant raid"<<endl;
                return mat;                    
            }
        }
        // --------------instant raid over-------------------
        // float ratio = cputime_j/remaining_j;
        // float h1 = cputime_j / n_j /(remaining_j - 10);
        depth_j = max_d;
        // cout<<"h value is "<<h1<<endl;
        cout<<"n_j value is "<<n_j<<endl;
        if(remaining_j > n_j_p*3/5){
            mode_j = 0;
            depth_j = 2;
        }else{
            cout<<"here"<<endl;
            float lasttime = 0;
            string lasttime_s = "";
            string lastdepth_s = "";
            int lastdepth = 0;
            ifstream tic_if("timer.txt");
            if(tic_if.good()){
                getline(tic_if,lasttime_s);
                lasttime = stof(lasttime_s);
                getline(tic_if,lastdepth_s);
                lastdepth = stoi(lastdepth_s);
                depth_j = lastdepth;
                if(lasttime - cputime_j < 3 && depth_j < max_d) depth_j++;
                // if(lasttime - cputime_j < 15) depth_j++;
                else if(lasttime - cputime_j > 6) depth_j--;
                
            }else{
                depth_j = max_d;
            }
            tic_if.close();
            ofstream tic_f("timer.txt");
            tic_f<<cputime_j<<endl<<depth_j;
            tic_f.close();
            // if( h1 < 0.11){
            //     mode_j = 1;
            //     depth_j = 2;
            // }
            // else if( h1 > 5){
            //     mode_j = 2;
            //     depth_j = 4;
            // }else{
            //     mode_j = 3;
            //     depth_j = 3;        
            // }
        } 
    }

    // modify depth here
    // depth_j = 3;
    // 
    // if(d>0) depth_j--;
    cout<<"current depth: "<<depth_j<<endl;
    
    if(remaining_j<=0 || depth_j==0) return "No move";
    int max_v = numeric_limits<int>::min();
    int copy_j = sizeof(char)*n_j_p;
    score_j = 0;
    for(int i = 0; i < n_j; i++){
        for(int j = 0; j<n_j; j++){
            if(state_j[i][j] == youplay_j) score_j+=cell_j[i][j];
            else if(state_j[i][j] == enemy_j) score_j -= cell_j[i][j];
        }
    }
    
    // ----------------------Prunning Section----------------------------

    for(int i = 0; i < n_j; i++){
        for(int j = 0; j<n_j; j++){
            if(state_j[i][j] == '.'){

                char** cur_state = (char **)malloc(n_j*sizeof(char*));
                for(int k=0;k<n_j;k++){
                    cur_state[k]=(char *) malloc(n_j);
                    memcpy(cur_state[k],state_j[k], n_j);
                }
                // int size = sizeof(cur_state);
                int eval_t = score_j;
                int type = calc(cur_state,i,j, youplay_j, eval_t);
                // for(int i1 = 0; i1 < n_j; i1++){
                //     for(int j1 = 0; j1 < n_j; j1++){
                //         cout<<cur_state[i1][j1]<<" ";
                //     }
                //     cout<<endl;
                // }
                int cur_val = min_n(cur_state, a, b, 1, remaining_j-1, -eval_t);
                if(cur_val > max_v){
                    ret[0] = i;
                    ret[1] = j;
                    ret[2] = type;
                    max_v = cur_val;
                }
                // if( max_v >= b){
                //     ret[0] = i;
                //     ret[1] = j;
                //     ret[2] = type;
                //     string mat = "";
                //     // calc(state_j, ret[0], ret[1], youplay_j);
                //     for(int ik = 0; ik < n_j; ik++){
                //         for(int jk = 0; jk<n_j-1; jk++){
                //             mat.push_back(cur_state[ik][jk]);
                //             mat.push_back(' ');
                //         }
                //         mat.push_back(cur_state[ik][n_j-1]);
                //         mat.push_back('\n');
                //     }
                //     for(int k = 0; k < n_j; k++)
                //         delete cur_state[k];
                //     delete cur_state;   
                //     return mat;
                // }
                for(int k = 0; k < n_j; k++)
                    delete cur_state[k];
                delete cur_state;   
                a = max(a,max_v);
            }
        }
    }
    calc(state_j, ret[0], ret[1], youplay_j, score_j);
    string mat = "";
    for(int ik = 0; ik < n_j; ik++){
        for(int jk = 0; jk<n_j; jk++){
            mat.push_back(state_j[ik][jk]);
        }
        mat.push_back('\n');
    }
    return mat;
}

int Jason::max_n(char** s, int a, int b, int d, int r, int eval){
    // cout<<"max node in depth: "<<d<<endl;
    if(r<=0 || d==depth_j) return eval;
    int max_v = numeric_limits<int>::min();
    int copy_j = sizeof(char)*n_j_p;
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
        int temp = 0;
        calc(cur_state,nodes[i].x,nodes[i].y, youplay_j, temp);
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
        int temp = 0;
        calc(cur_state,nodes[i].x,nodes[i].y, enemy_j, temp);
        min_v = min(max_n(cur_state, a, b, d+1, r-1, -nodes[i].eval_t), min_v);
        for(int k = 0; k < n_j; k++)
            delete cur_state[k];
        delete cur_state;   
        if( min_v <= a) return min_v;
        b = min(b,min_v);

    }
    return min_v;
}
int Jason::utility(char** s){
    int ret = 0;
    for(int i = 0; i < n_j; ++i){
        for(int j = 0; j<n_j; j++){
            if(s[i][j] == youplay_j) ret +=cell_j[i][j];
            if(s[i][j] == enemy_j) ret -=cell_j[i][j];
        }
    }
    return ret;
}

int Jason::calc(char** s, int y, int x, char yp, int &eval){
    char ep = 'X';
    if( yp == 'X') ep = 'O';
    bool raid = false;
    bool raid_avail = false;
    int addition = largest_j/5;
    // if(remaining_j<n_j_p/3) addition = largest_j/6;
    // if(remaining_j<n_j_p/5) addition = 0;
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
                // eval += 2* cell_j[y][x-1];
                eval += 2* cell_j[y][x-1] + addition;
                raid = true;
            }
        }
        if(x<n_j-1){
            if (s[y][x+1] == ep){
                s[y][x+1] = yp;
                // eval += 2* cell_j[y][x+1];
                eval += 2* cell_j[y][x+1]+ addition;
                raid = true;
            }
        }
        if(y>0){
            if (s[y-1][x] == ep){
                s[y-1][x] = yp;
                // eval += 2* cell_j[y-1][x];
                eval += 2* cell_j[y-1][x]+ addition;
                raid = true;
            }
        }
        if(y<n_j-1){
            if (s[y+1][x] == ep){
                s[y+1][x] = yp;
                // eval += 2* cell_j[y+1][x];
                eval += 2* cell_j[y+1][x]+ addition;
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
    int depth = 0;
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