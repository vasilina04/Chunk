#pragma once

#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <optional> 


template <typename T, size_t N>
class ChunkList {
private:
    struct Chunk {
        std::vector<T> elements;
        Chunk* next;
        Chunk* prev;

        explicit Chunk(Chunk* prev_ptr = nullptr, Chunk* next_ptr = nullptr) : elements(N), next(next_ptr), prev(prev_ptr) {}
    };

    std::list<Chunk*> chunks;
    size_t total_size;

public:
    ChunkList() : total_size(0) {}

    void push_back(const T& value) {
        if (chunks.empty() || chunks.back()->elements.size() == N) {
            chunks.push_back(new Chunk(chunks.empty() ? nullptr : chunks.back()));
        }
        chunks.back()->elements.push_back(value);
        ++total_size;
    }


    void pop_back() {
        if (chunks.empty()) {
            return;
        }
        chunks.back()->elements.pop_back();
        --total_size;
        if (chunks.back()->elements.empty()) {
            delete chunks.back();
            chunks.pop_back();
        }
    }

    size_t len() const {
        return total_size;
    }

    bool is_full() const {
        if (chunks.empty()) {
            return false;
        }
        return chunks.back()->elements.size() == N;
    }

    bool is_empty() const {
        return total_size == 0;
    }


    bool push_front(const T& value) {
        if (chunks.empty() || chunks.front()->elements.size() == N){
            return false;
        }
        chunks.front()->elements.insert(chunks.front()->elements.begin(), value);
        ++total_size;
        return true;
    }

    std::optional<T> pop_front(){
        if (chunks.empty()) {
            return std::nullopt;
        }
        T value = chunks.front()->elements.front();
        chunks.front()->elements.erase(chunks.front()->elements.begin());
        --total_size;
        if (chunks.front()->elements.empty()){
            delete chunks.front();
            chunks.pop_front();
        }
        return value;
    }

    std::optional<T> get(size_t i){
        size_t offset = 0;
        for (const auto& chunk : chunks){
            if (i < offset + chunk->elements.size()) {
                return chunk->elements[i - offset];
            }
            offset += chunk->elements.size();
        }
        return std::nullopt;
    }

    std::optional<T> remove(size_t i){
        size_t offset = 0;
        for (auto it = chunks.begin(); it != chunks.end(); ++it) {
            if (i < offset + (*it)->elements.size()) {
                T value = (*it)->elements[i - offset];
                (*it)->elements.erase((*it)->elements.begin() + (i - offset));
                --total_size;
                if ((*it)->elements.empty()) {
                    delete (*it);
                    chunks.erase(it);
                }
                return value;
            }
            offset += (*it)->elements.size();
        }
        return std::nullopt;
    }

    class iterator {
    public:
        iterator(ChunkList<T, N>& list, typename std::list<Chunk*>::iterator chunkIt, typename std::vector<T>::iterator elemIt) :
            chunk_list(list), chunk_it(chunkIt), elem_it(elemIt) {}

        iterator& operator++() {
            ++elem_it;
            if (elem_it == (*chunk_it)->elements.end()) {
                ++chunk_it;
                if (chunk_it != chunk_list.chunks.end()) {
                    elem_it = (*chunk_it)->elements.begin();
                }
            }
            return *this;
        }

        iterator& operator--() {
            if (elem_it == (*chunk_it)->elements.begin()) {
                --chunk_it;
                if (chunk_it != chunk_list.chunks.end()) {
                    elem_it = (*chunk_it)->elements.end();
                }
            }
            --elem_it;
            return *this;
        }

        bool operator!=(const iterator& other) const {
            return chunk_it != other.chunk_it || elem_it != other.elem_it;
        }

        T& operator*() {
            return *elem_it;
        }
    private:
        ChunkList<T, N>& chunk_list;
        typename std::list<Chunk*>::iterator chunk_it;
        typename std::vector<T>::iterator elem_it;
    };

    iterator begin() {
        return iterator(*this, chunks.begin(), chunks.empty() ? typename std::vector<T>::iterator() : chunks.front()->elements.begin());
    }

    iterator end() {
        return iterator(*this, chunks.end(), typename std::vector<T>::iterator());
    }
};