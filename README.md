# trie-visualize
An attempt at visualizing tries.


## Compiling:

```bash
    $> cd build/
    $> make
```

## Executing:

Locate the trie vector locally before generating the svg.
```cpp
    vector<string> words {"abcde", "abc", "a"};

    Trie* obj = new Trie();

    int length = words.size();
```

```bash
    $> ./tries
```

The dot file and svg generated in `build` folder.

## Output:

![Sample.svg](/sample-outputs/sample1.svg)
![Sample.svg](/sample-outputs/sample2.svg)

![Sample.svg](/sample-outputs/sample3.svg)
