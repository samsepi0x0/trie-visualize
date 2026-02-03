#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <fstream>

using namespace std;

class TrieNode {
    public:
        vector<TrieNode*> arr;
        bool wordEnd;

        TrieNode() {
            arr.resize(26);
            for (int i = 0; i < 26; i++) {
                arr[i] = NULL;
            }
            wordEnd = false;
        }
};

struct Edge {
    TrieNode* parent;
    TrieNode* child;
    int index;
};

class Trie {
    public:
        TrieNode* root;
        int wordCount = 0;
        unordered_map<TrieNode*, int> nodeMap;
        vector<Edge> parentMap;

        Trie() {
            root = new TrieNode();
        }

        void insert(string word) {
            TrieNode* curr = root;
            int length = word.length();
            for (int i = 0; i < length; i++) {
                int ind = word[i] - 'a';
                if (curr->arr[ind] == NULL) {
                    TrieNode* temp = new TrieNode();
                    curr->arr[ind] = temp;
                }
                curr = curr->arr[ind];
            }
            curr->wordEnd = true;
            wordCount += 1;
        }

        bool search(string word) {
            TrieNode* curr = root;
            int length = word.length();
            for (int i = 0; i < length; i++) {
                int ind = word[i] - 'a';
                if (curr->arr[ind] == NULL)
                    return false;
                curr = curr->arr[ind];
            }
            return curr->wordEnd;
        }

        int getNodeCount() {
            TrieNode* curr = root;
            queue<TrieNode*> q;
            q.push(curr);

            nodeMap.clear();
            parentMap.clear();
            int id = 0;

            while (!q.empty()) {
                int size = q.size();
                for (int i = 0; i < size; i++) {
                    TrieNode* t = q.front();
                    nodeMap[t] = id++;
                    
                    q.pop();
                    bool children = false;

                    for (int j = 0; j < 26; j++) {
                        if (t->arr[j] != NULL) {
                            parentMap.push_back({t, t->arr[j], j});
                            if (t->arr[j]->wordEnd) {
                                parentMap.push_back({t, NULL, j});
                            }
                            q.push(t->arr[j]);
                            children = true;
                        }
                    }

                    if (!children)
                        nodeMap[t] *= -1;
                }
            }
            return id;
        }

        void generateDot() {
            ofstream gr("trie.dot");

            gr << "digraph g { \n" << endl;

            gr << "fontname = \"Helvetica, Arial, sans-serif\"" << endl;
            gr << "node [fontname = \"Helvetica, Arial, sans-serif\"] " << endl;
            gr << "edge [fontname = \"Helvetica, Arial, sans-serif\"] " << endl;
            gr << "graph [nodesep=0.2, ranksep=0.4];" << endl;
            gr << "graph [rankdir = \"TB\"]; " << endl; 
            gr << "node [ " << endl;
            gr << "\tfontsize = \"12\"" << endl;
            gr << "\tshape = \"ellipse\"" << endl;
            gr << "]; " << endl;
            gr << "edge []; " << endl;

            [[maybe_unused]] int nodeCount = getNodeCount();

            // setting node info
            unordered_map<int, vector<int>> nodeLetters;
            unordered_map<int, bool> endNode;

            for (int i = 0; i < (int)(parentMap.size()); i++) {
                TrieNode* parent = parentMap[i].parent;
                TrieNode* child = parentMap[i].child;
                int index = parentMap[i].index;
                int nodeID = nodeMap[parent];
                
                if (nodeLetters.count(nodeID) == 0) {
                    vector<int> temp;
                    nodeLetters[nodeID] = temp;
                }
                nodeLetters[nodeID].push_back(index);
            }

            for (auto it = nodeLetters.begin(); it != nodeLetters.end(); it++) {
                int nodeID = it->first;
                gr << "\"node" << nodeID << "\" [ " << endl;
                gr << "\tshape = \"plaintext\"" << endl;
                gr << "\tfontsize = \"16\"" << endl;
                
                gr << "\tlabel = <" << endl;
                gr << "\t\t" << "<TABLE CELLPADDING=\"10\" BORDER=\"1\" CELLBORDER=\"2\" CELLSPACING=\"5\">" << endl;
                gr << "\t\t<TR>" << endl;
                
                int n = (int)(it->second.size());
                for (int i = 0; i < n; i++) {
                    int j = it->second[i];
                    gr << "\t\t\t<TD ALIGN=\"CENTER\" VALIGN=\"MIDDLE\" PORT = \"f" << abs(j) << "\"";
                    if (i != n-1 && it->second[i] == it->second[i + 1]) {
                        gr << " BGCOLOR=\"gray\"> ";
                        i += 1;
                    } else {
                        gr << "> ";
                    }
                    gr << (char)(65 + j) << " </TD>" << endl;
                }

                gr << "\t\t</TR>" << endl << "\t\t</TABLE>" << endl;
                gr << "\t>" << endl;

                gr << "]" << endl;
            }
            
            // adding edges
            int id = 0;
            for (int i = 0; i < (int)parentMap.size(); i++) {
                TrieNode* p = parentMap[i].parent;
                TrieNode* c = parentMap[i].child;
                int index = parentMap[i].index;

                string nodeP = "node" + to_string(nodeMap[p]);
                if (c == NULL || nodeMap[c] < 0) {
                    continue;
                } 

                string nodeC = "node" + to_string(nodeMap[c]);

                // cout << nodeP << "\t" << nodeC << "\t" << (char)(index + 65) << endl;

                gr << "\"" << nodeP << "\":f" << index << " -> \"" << nodeC << "\" [ id = " << id++ << " ];\n";

            }
            gr << "\n}";
            gr.close();
        }

        void generateSVG() {
            system("dot -Tsvg trie.dot -o trie.svg");
            cout << "SVG created." << endl;
        }
};

int main() {
    vector<string> words {"harry"};

    Trie* obj = new Trie();

    int length = words.size();

    if (length == 0) {
        cout << "Error: Cannot create empty trie diagram." << endl;
        return 0;
    }

    for (int i = 0; i < length; i++) {
        obj->insert(words[i]);
    }

    obj->generateDot();
    obj->generateSVG();

    return 0;
}