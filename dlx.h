#ifndef DLX_H
#define DLX_H

#include <vector>
#include <iostream>

class Dlx {
    public:
        Dlx(const std::vector<std::vector<bool>>& matrix,
            const std::vector<std::string>& names);

        ~Dlx() = default;
        Dlx(const Dlx& other) = default;
        Dlx& operator=(const Dlx& other) = default;
        Dlx& operator=(Dlx&& other) = default;

        void search(int k = 0);
        void search_log(int k = 0);
        bool search_first(int k = 0);

        std::vector<std::string> get_ans();
        void print_internals();

    private:
        void cover_column(int c);
        void uncover_column(int c);
        std::string produce_rows();

        std::vector<int> O;
        std::vector<int> L;
        std::vector<int> R;
        std::vector<int> U;
        std::vector<int> D;
        std::vector<int> C;
        std::vector<int> S;
        std::vector<std::string> N;
        std::vector<std::string> answers;
        int h;

};

#endif /* DLX_H */
