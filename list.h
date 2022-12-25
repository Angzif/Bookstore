//
// Created by 17111 on 2022/12/25.
//

#ifndef BOOKSTORE_FINAL_LIST_H
#define BOOKSTORE_FINAL_LIST_H
#include <iostream>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <string>
using namespace std;
class String {
private:
    char ch[70];
public:
    String() = default;

    String(const char *s) {
        int len = strlen(s);
        if (len > 64)
            exit(-1);
        for (int i = 0; i < len; i++) {
            ch[i] = s[i];
        }
        ch[len] = '\0';
    }

    ~String() = default;

    friend bool operator<(const String &obj1, const String &obj2) {
        return (strcmp(obj1.ch, obj2.ch) < 0);
    }

    friend bool operator==(const String &obj1, const String &obj2) {
        return (strcmp(obj1.ch, obj2.ch) == 0);
    }

    friend bool operator<=(const String &obj1, const String &obj2) {
        return (obj1 < obj2 || obj1 == obj2);
    }

    friend bool operator>(const String &obj1, const String &obj2) {
        return (!(obj1 <= obj2));
    }

    friend bool operator>=(const String &obj1, const String &obj2) {
        return (obj1 > obj2 || obj1 == obj2);
    }

    friend std::ostream &operator<<(std::ostream &x, const String &y) {
        x << y.ch;
        return x;
    }
};

class Node {
public:
    String name;
    int value;

    Node() = default;

    Node(const String &Name, const int Value) {
        name = Name;
        value = Value;
    }

    Node(const Node &_node) {
        name = _node.name;
        value = _node.value;
    }

    ~Node() = default;
};

bool operator<(const Node &a, const Node &b) {
    if (!(a.name == b.name)) {
        return (a.name < b.name);
    }
    return a.value < b.value;
}

bool operator>(const Node &a, const Node &b) {
    if (!(a.name == b.name)) {
        return !(a.name < b.name);
    }
    return a.value > b.value;
}

bool operator==(const Node &a, const Node &b) {
    if (!(a.name == b.name)) {
        return false;
    }
    return a.value == b.value;
}

bool operator<=(const Node &a, const Node &b) {
    return !(a > b);
}

bool operator>=(const Node &a, const Node &b) {
    return !(a < b);
}

struct Index {
    Node sup;
    Node inf;
    int Next = -1;
    int size = 0;
    Node data[1200];

};
class Block{
private:
    const int max = 1000;
    const int min = 500;
    int block_number = -1;
    fstream file;
    void read_data(int pos,Index& data){
        if (pos < 0)
            exit(-1);
        file.seekg(pos * sizeof(Index) + sizeof(int));
        file.read(reinterpret_cast<char*>(&data),sizeof(Index));
    }
    void write_data(int pos, const Index &data) {
        if (pos < 0)
            exit(-1);
        file.seekp(pos * sizeof(Index) + sizeof(int));
        file.write(reinterpret_cast<const char *>(&data), sizeof(Index));
    }
    void read_index(int pos,Index& data){
        if (pos < 0)
            exit(-1);
        file.seekg(pos * sizeof(Index) + sizeof(int));
        file.read(reinterpret_cast<char*>(&data.sup),sizeof(data.sup));
        file.read(reinterpret_cast<char*>(&data.inf),sizeof(data.inf));
        file.read(reinterpret_cast<char*>(&data.Next),sizeof(data.Next));
        file.read(reinterpret_cast<char*>(&data.size),sizeof(data.size));
    }
    void split(int pos,Index& data){
        block_number++;
        static Index now;
        static Index old_one;
        for (int i = min;i < data.size;++i){
            now.data[i - min] = data.data[i];
            old_one.data[i - min] = data.data[i - min];
        }
        now.size = min + 1;
        old_one.size = min;
        now.Next = data.Next;
        old_one.Next = block_number;
        now.inf = now.data[0];
        now.sup = now.data[now.size - 1];
        old_one.inf = old_one.data[0];
        old_one.sup = old_one.data[old_one.size - 1];
        write_data(pos,old_one);
        write_data(block_number,now);
    }
    void combine(int pos,Index& data){
        int next_one = data.Next;
        if (next_one == -1){
            write_data(pos,data);
            return;
        }
        static Index nxt;
        read_data(next_one,nxt);
        if (data.size + nxt.size <= max){
            for (int i = data.size;i < data.size + nxt.size;++i){
                data.data[i] = nxt.data[i - data.size];
            }
            data.size += nxt.size;
            data.Next = nxt.Next;
            data.inf = data.data[0];
            data.sup = data.data[data.size - 1];
        }
        write_data(pos,data);
    }
    void internal_insert(int pos,const Node& data){
        static Index now;
        read_data(pos,now);
        int tmp = -1;
        for (int i = 0;i < now.size;i++){
            if (data < now.data[i]){
                tmp = i;
                break;
            }
            if (data == now.data[i])
                return;
        }
        now.size++;
        if (tmp == -1){
            now.data[now.size - 1] = data;
        }
        else {
            for (int i = now.size - 1;i > tmp;--i){
                now.data[i] = now.data[i - 1];
            }
            now.data[tmp] = data;
        }
        if (now.size > max){
            split(pos,now);
        }
        else {
            now.inf = now.data[0];
            now.sup = now.data[now.size - 1];
            write_data(pos,now);
        }
    }
    void internal_deltete(int pos,const Node& data){
        static Index now;
        read_data(pos,now);
        for (int i = 0;i < now.size;i++){
            if (now.data[i] > data){
                break;
            }
            if (now.data[i] == data){
                for (int j = i + 1;j < now.size;++j){
                    now.data[j - 1] = now.data[j];
                }
                now.size--;
                if (now.size == 0){
                    write_data(pos,now);
                    return;
                }
                now.inf = now.data[0];
                now.sup = now.data[now.size - 1];
                if (now.size < min){
                    combine(pos,now);
                }
                else {
                    write_data(pos,now);
                }
                return;
            }
        }
    }
public:
    Block(){
        file.open("test");
        if (file){
            file.seekg(0);
            file.read(reinterpret_cast<char*>(&block_number),sizeof(block_number));
        }
        else {
            file.open("test",fstream::out);
            file.close();
            file.open("test");
            block_number = -1;
            file.seekp(0);
            file.write(reinterpret_cast<char*>(&block_number),sizeof(block_number));
            file.seekp(sizeof(int));
            static Index tmp;
            file.write(reinterpret_cast<char*>(&tmp),sizeof(tmp));
        }
    }
    ~Block(){
        file.seekp(0);
        file.write(reinterpret_cast<char*>(&block_number),sizeof(block_number));
        file.close();
    }
    void insert(const Node& data){
        static Index now;
        if (block_number == -1){
            ++block_number;
            now.size = 1;
            now.inf = data;
            now.sup = data;
            now.data[0] = data;
            write_data(0,now);
            return;
        }
        read_index(0,now);
        if (data < now.inf){
            internal_insert(0,data);
            return;
        }
        static Index nxt;
        int position_now = 0;
        while (now.Next != -1){
            read_index(now.Next,nxt);
            if (data > now.inf && data < nxt.inf){
                internal_insert(position_now,data);
                return;
            }
            position_now = now.Next;
            read_data(position_now,now);
        }
        internal_insert(position_now,data);
    }
    void del(const Node& data){
        static Index now;
        read_index(0,now);
        int position_now = 0;
        while (position_now != -1){
            if (data >= now.inf && data <=now.sup){
                internal_deltete(position_now,data);
                return;
            }
            if (data < now.inf){
                return;
            }
            position_now = now.Next;
            read_index(position_now,now);
        }
    }
    void find(const String& index){
        static Index now;
        int i = 0;
        bool has = false;
        while (i != -1){
            read_index(i,now);
            if (now.size == 0){
                i = now.Next;
                continue;
            }
            if ((now.inf.name <= index) && (now.sup.name >= index)){
                read_data(i,now);
                for (int j = 0;j < now.size;j++){
                    if (now.data[j].name == index){
                        has = true;
                        cout << now.data[j].value << ' ';
                    }
                }
            }
            else if(index < now.inf.name){
                break;
            }
            i = now.Next;
        }
        if (!has){
            cout << "null\n";
        }
        else {
            cout << '\n';
        }
    }
};
#endif //BOOKSTORE_FINAL_LIST_H
