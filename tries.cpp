/*
    trie implementation successful,

    Todo:
    shrink nodes that are not being used
        this needs major revamp as current data structure choice would not support this feature.
    animate the graph
        This will require generating a svg at each step after every node and edge gets added, and then convert the sequence of images to a gif using ffmpeg 
*/

#include <bits/stdc++.h>

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
        unordered_map<int, TrieNode*> reverseNodeMap; // remove this, do not need this.

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
            /*
                Todo: find parent of each node, along with the index it points to. (parent, index) -> (child, index)
                
            */
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
                    reverseNodeMap[id] = t;
                    nodeMap[t] = id;
                    id += 1;
                    
                    q.pop();

                    for (int j = 0; j < 26; j++) {
                        if (t->arr[j] != NULL) {
                            parentMap.push_back({t, t->arr[j], j});
                            q.push(t->arr[j]);
                        }
                    }
                }
            }
            return id;
        }

        void generateDot() {
            ofstream gr("tri.dot");

            gr << "digraph g { \n" << endl;

            gr << "fontname=\"Helvetica,Arial,sans-serif\" " << endl;
            gr << "node [fontname=\"Helvetica,Arial,sans-serif\"] " << endl;
            gr << "edge [fontname=\"Helvetica,Arial,sans-serif\"] " << endl;
            gr << "graph [nodesep=0.2, ranksep=0.4];\n";
            gr << "graph [ rankdir = \"TB\" ]; " << endl; 
            gr << "node [ " << endl;
            gr << "fontsize = \"12\"" << endl;
            gr << "shape = \"ellipse\"" << endl;
            gr << "]; " << endl;
            gr << "edge []; " << endl;

            int nodeCount = getNodeCount();

            // setting node info
            for (int i = 0; i < nodeCount; i++) {
                gr << "\"node" << i << "\" [ " << endl;
                gr << "label = \"";

                for (int j = 0; j < 26; j += 2) {
                    gr << "{" << "<f" << j << "> " << (char)(65 + j) << " | " << "<f" << j + 1 << "> " << (char)(65 + j + 1) << "}";
                    if (j != 24)
                        gr << " | ";
                }
                gr << "\"\nshape = \"record\"" << endl;
                gr << "];" << endl; 
            }

            // adding edges
            int id = 0;
            for (int i = 0; i < (int)parentMap.size(); i++) {
                TrieNode* p = parentMap[i].parent;
                TrieNode* c = parentMap[i].child;
                int index = parentMap[i].index;

                string nodeP = "node" + to_string(nodeMap[p]);
                string nodeC = "node" + to_string(nodeMap[c]);

                cout << nodeP << "\t" << nodeC << "\t" << (char)(index + 65) << endl;

                gr << "\"" << nodeP << "\":f" << index << " -> \"" << nodeC << "\":f" << index << " [ id = " << id++ << " ];\n";

            }
            // for (auto it = parentMap.begin(); it != parentMap.end(); it++) {
            //     TrieNode* parent = it->first;
            //     TrieNode* child = it->second.first;
            //     int value = it->second.second;

            //     string nodeP = "node" + to_string(nodeMap[parent]);
            //     string nodeC = "node" + to_string(nodeMap[child]);

            //     gr << "\"" << nodeP << "\":" << "f" << value << " -> \"" << nodeC << "\":f" << value << "\n[ id = " << id << " ];" << endl;
            //     cout << nodeP << "\t" << nodeC << "\t" << (char)(value + 65) << endl;
            //     id++;
            // }

            gr << "\n}";
            

            gr.close();
        }

        void generateSVG() {
            system("dot -Tsvg tri.dot -o trie.svg");
            cout << "SVG created." << endl;
        }
};


int main() {
    vector<string> words {"abcd", "abe"};

    Trie* obj = new Trie();
    int length = words.size();
    for (int i = 0; i < length; i++) {
        obj->insert(words[i]);
    }

    obj->generateDot();
    obj->generateSVG();

    return 0;
}