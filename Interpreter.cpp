#include<bits/stdc++.h>
using namespace std;
#define ll long long

bool is_space(char x){
    if(x == ' ') return true;
    else return false;
}

bool is_paren(char x){
    if(x == '(' || x == ')') return true;
    else return false;
}

string get_paren(const string& s, ll i){
    if(s[i] == '(') return "(";
    else return ")";
}

struct Environment;

struct Node{
    string name;
    ll children = 0;
    Node* child[100];
    struct Environment* eptr;
};

Node* new_node(string x){
    Node* node = new Node;
    node->name = x;
    node->children = 0;
    return node;
}

void add_child(Node* parent_node, Node* child_node){
    parent_node->children++;
    parent_node->child[parent_node->children-1] = child_node;
}

string get_identifier(const string& s, ll& i) {
    int j = i;
    while(j < s.size() && !isspace(s[j]) && !is_paren(s[j])) j++;
    string res = s.substr(i, j - i);
    i = j;
    return res;
}

queue<string> tokenize(const string& s){
    queue<string> temp;
    ll i = 0;
    for(ll j = 0;j < s.length();j++){
        if(s[j] == ' '){
            temp.push(s.substr(i, j-i));
            i = j+1;
        }
    }
    temp.push(")");
    /*ll len = s.size();
    ll i = 0;
    while(i < len) {
        if(s[i] == ' ') continue;
        else if(s[i] == '(' || s[i] == ')'){
            temp.push(get_paren(s, i));
        }
        else
            temp.push(get_identifier(s, i));
    }*/
    return temp;
}

Node* parser(queue<string>& tokens){
    string temp = tokens.front();//cout << tokens.size() << endl;
    tokens.pop();
    
    if(is_paren(temp[0])){
        Node* node = new_node("group");
        
        while(tokens.front() != ")"){
            if(tokens.size() == 0) break;
            add_child(node, parser(tokens));
        }
        tokens.pop();
        
        return node;
    }
    else{
        Node* node = new_node(temp);
        
        return node;
    }
}

void print_node(Node* x){
    if(x->name == "group"){
        cout << "( ";
        for(ll i = 0;i < x->children;i++){
            print_node(x->child[i]);
            cout << " ";
        }
        cout << ")";
    }
    else if(x->name == "loop_output"){
        for(ll i = 0;i < x->children;i++){
            cout << x->child[i]->name << endl;
        }
    }
    else{
        cout << x->name;
    }
}

typedef struct Environment{
    unordered_map<string, Node*> m;
    Environment* outer_env;
} Environment;

bool is_number(string x){
    return isdigit(x[0]);
}

Environment* Find_env(Environment* current, string s)
{
    if(current->m.find(s) == current->m.end()){
        return Find_env(current->outer_env, s);
    }
    else{
        return current;
    }
}

Node* sum_func(queue<Node*> args){
    ll res = 0;
    while(!args.empty()){
        Node* temp = args.front();
        args.pop();
        res += stoi(temp->name);
    }
    return new_node(to_string(res));
}

Node* sub_func(queue<Node*> args){
    ll res = stoi(args.front()->name);
    args.pop();
    res -= stoi(args.front()->name);
    args.pop();
    return new_node(to_string(res));
}

Node* mul_func(queue<Node*> args){
    ll res = stoi(args.front()->name);
    args.pop();
    res *= stoi(args.front()->name);
    args.pop();
    return new_node(to_string(res));
}

Node* div_func(queue<Node*> args){
    ll res = stoi(args.front()->name);
    args.pop();
    if(args.front()->name == "0"){
        args.pop();
        return new_node("Division by zero is not allowed.");
    }
    res /= stoi(args.front()->name);
    args.pop();
    return new_node(to_string(res));
}

Node* lesser_func(queue<Node*> args){
    ll x = stoi(args.front()->name);
    args.pop();
    ll y = stoi(args.front()->name);
    args.pop();
    if(x < y) return new_node("TRUE");
    else return new_node("FALSE");
}

Node* greater_func(queue<Node*> args){
    ll x = stoi(args.front()->name);
    args.pop();
    ll y = stoi(args.front()->name);
    args.pop();
    if(x > y) return new_node("TRUE");
    else return new_node("FALSE");
}

Node* equal_func(queue<Node*> args){
    ll x = stoi(args.front()->name);
    args.pop();
    ll y = stoi(args.front()->name);
    args.pop();
    if(x == y) return new_node("TRUE");
    else return new_node("FALSE");
}

Node* or_func(queue<Node*> args){
    ll x = stoi(args.front()->name);
    args.pop();
    ll y = stoi(args.front()->name);
    args.pop();
    return new_node(to_string(x|y));
}

Node* and_func(queue<Node*> args){
    ll x = stoi(args.front()->name);
    args.pop();
    ll y = stoi(args.front()->name);
    args.pop();
    return new_node(to_string(x&y));
}

Node* xor_func(queue<Node*> args){
    ll x = stoi(args.front()->name);
    args.pop();
    ll y = stoi(args.front()->name);
    args.pop();
    return new_node(to_string(x^y));
}

Node* not_func(queue<Node*> args){
    ll x = stoi(args.front()->name);
    args.pop();
    return new_node(to_string(~x));
}

Node* begin_func(queue<Node*> args){
    ll x;
    while(!args.empty()){
        x = stoi(args.front()->name);
        args.pop();
    }
    return new_node(to_string(x));
}

Node* list_func(queue<Node*> args){
    Node* node = new_node("group");
    while(!args.empty()){
        add_child(node, args.front());
        args.pop();
    }
    return node;
}

Node* append_func(queue<Node*> args){
    Node* node1 = args.front();
    args.pop();
    Node* node2 = args.front();
    args.pop();
    Node* node = new_node("group");
    for(ll i = 0;i < node1->children;i++) add_child(node, node1->child[i]);
    for(ll i = 0;i < node2->children;i++) add_child(node, node2->child[i]);
    return node;
}

Node* evaluate(Node* ,Environment* );

Node* operate(Node* procedure, Node* node, queue<Node*> args, Environment* env){
    
    if(procedure->name == "+"){
        return sum_func(args);
    }
    else if(procedure->name == "-"){
        return sub_func(args);
    }
    else if(procedure->name == "*"){
        return mul_func(args);
    }
    else if(procedure->name == "/"){
        return div_func(args);
    }
    else if(procedure->name == "<"){
        return lesser_func(args);
    }
    else if(procedure->name == ">"){
        return greater_func(args);
    }
    else if(procedure->name == "=="){
        return equal_func(args);
    }
    else if(procedure->name == "|"){
        return or_func(args);
    }
    else if(procedure->name == "&"){
        return and_func(args);
    }
    else if(procedure->name == "^"){
        return xor_func(args);
    }
    else if(procedure->name == "~"){
        return not_func(args);
    }
    else if(procedure->name == "begin"){
        return begin_func(args);
    }
    else if(procedure->name == "list"){
        return list_func(args);
    }
    else if(procedure->name == "append"){
        return append_func(args);
    }
    else if(procedure->name == "lambda_new"){
        Node* parameter_node = node->child[0];
        Node* body_node = node->child[1];
        Environment* new_env = new Environment;
        new_env->outer_env = env;
        for(auto i:env->m){
            new_env->m[i.first] = i.second;
        }
        for(ll i = 0;i < parameter_node->children;i++){
            new_env->m[parameter_node->child[i]->name] = args.front();
            args.pop();
        }
        return evaluate(body_node, new_env);
        
    }
    return new_node("Error");
}

Node* quote_func(Node* node){
    return node;
}

Node* make_container(Node* node){
    string container_type = node->child[0]->name;
    if(container_type == "make-array"){
        Node* container_node = new_node("array");
        for(ll i = 0;i < node->child[1]->children;i++){
            add_child(container_node, node->child[1]->child[i]);
        }
        return container_node;
    }
    return new_node("Error");
}

Node* evaluate(Node* node, Environment* env){
    string node_type = node->name;
    if(node_type == "group"){
        Node* fchild = node->child[0];
        if(fchild->name == "quote"){
            return quote_func(node->child[1]);
        }
        else if(fchild->name == "define"){
            Node* variable = node->child[1];
            Node* data = evaluate(node->child[2], env);
            env->m[variable->name] = data;
            return new_node("ok");
        }
        else if(fchild->name == "set!"){
            Node* variable = node->child[1];
            Node* data = evaluate(node->child[2], env);
            env->m[variable->name] = data;
            return new_node("ok");
        }
        else if(fchild->name == "lambda"){
            Node* node_new = new_node("lambda_new");
            add_child(node_new, node->child[1]);
            add_child(node_new, node->child[2]);
            
            return node_new;
        }
        else if(fchild->name == "if"){
            if(evaluate(node->child[1], env)->name == "TRUE"){
                return evaluate(node->child[2], env);
            }
            else return evaluate(node->child[3], env);
        }
        else if(fchild->name == "group"){
            Node* procedure = evaluate(fchild, env);
            queue<Node*> args;
            for(ll i = 1;i < node->children;i++){
                args.push(evaluate(node->child[i], env));
            }
            
            Node* res_node = operate(env->m[procedure->name], procedure, args, env);
            return res_node;
        }
        else if(fchild->name == "loop"){
            string variable = node->child[2]->name;
            Node* func_node = node->child[8];
            Node* node_new = new_node("loop_output");
            for(ll i = stoi(node->child[4]->name); i <= stoi(node->child[6]->name);i++){
                env->m[variable] = new_node(to_string(i));
                add_child(node_new, evaluate(func_node, env));
            }
            return node_new;
        }
        else if(fchild->name == "setf"){
            string variable = node->child[1]->name;
            env->m[variable] = make_container(node->child[2]);
            return new_node("ok");
        }
        else if(fchild->name == "aref"){
            string variable = node->child[1]->name;
            return env->m[variable]->child[(stoi(node->child[2]->name))];
        }
        else{
            Node* procedure = evaluate(node->child[0], env);
            queue<Node*> args;
            for(ll i = 1;i < node->children;i++){
                args.push(evaluate(node->child[i], env));
            }
            
            Node* res_node = operate(env->m[procedure->name], procedure, args, env);
            return res_node;
            
        }
    }
    else{
        if(is_number(node_type)){
            return node;
        }
        else{
            return env->m[node_type];
            /*Environment* ENV = Find_env(env, node_type);
            return ENV->m[node_type];*/
        }
    }
    return new_node("NULL");
}

void env_mapping(Environment& env){
    //env->outer_env = NULL;
    env.m["+"] = new_node("+");
    env.m["-"] = new_node("-");
    env.m["*"] = new_node("*");
    env.m["/"] = new_node("/");
    env.m["<"] = new_node("<");
    env.m[">"] = new_node(">");
    env.m["=="] = new_node("==");
    env.m["|"] = new_node("|");
    env.m["&"] = new_node("&");
    env.m["^"] = new_node("^");
    env.m["~"] = new_node("~");
    env.m["begin"] = new_node("begin");
    env.m["list"] = new_node("list");
    env.m["append"] = new_node("append");
    env.m["lambda_new"] = new_node("lambda_new");
}

int main(){
    Environment global_env;
    env_mapping(global_env);
    
    cout << "Start\n";
    
    while(true){
        cout << "Input> ";
        string s;
        getline(cin, s);
        queue<string> tokens = tokenize(s);
        queue<string> temp = tokens;

        if(s == "exit") break;
        
        Node* root_node = parser(tokens);
        
        Node* res = evaluate(root_node, &global_env);
        
        cout << "Output> ";
        print_node(res);
        cout << endl;
    }
}
// ( setf a ( make-array ( 1 2 3 ) ) )
// ( aref a 2 )

