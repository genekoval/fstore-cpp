#pragma once

#include <algorithm>
#include <ostream>

namespace fstore {
    template <class Derived, class T, int ColumnCount>
    class table {
    protected:
        typedef std::array<std::string, ColumnCount> row_t;
        typedef T value_t;
        static const int column_count = ColumnCount;
    private:
        std::array<int, ColumnCount> column_widths;
        std::vector<row_t> rows;

        row_t& new_row() {
            rows.emplace_back();
            return rows.back();
        }

        void print_break(std::ostream& os) const {
            os.fill('-');
            print_line(os, '-', '+', [](int i) -> std::string { return "-"; });
            os.fill(' ');
        }

        void print_line(std::ostream& os, char padding, char divider,
            std::function<std::string(int)> field
        ) const {
            int i = 0;

            while (i < ColumnCount) {
                os << padding;
                os.width(column_widths[i]);
                os << field(i) << padding;

                if (++i != ColumnCount) os << divider;
            }

            os << std::endl;
        }

        void print_row(std::ostream& os, const row_t& row) const {
            print_line(os, ' ', '|', [&row](int i) -> std::string {
                return row[i];
            });
        }

        void print(std::ostream& os) const {
            os.setf(std::ios::left, std::ios::adjustfield);

            print_row(os, Derived::headers);
            print_break(os);
            for (const auto& entry : rows) print_row(os, entry);
        }

        void resize() {
            for (int i = 0; i < ColumnCount; i++) {
                const int current_width = rows.back()[i].size();
                auto& greatest_width = column_widths[i];

                if (current_width > greatest_width)
                    greatest_width = current_width;
            }
        }
    public:
        table() {
            std::generate(column_widths.begin(), column_widths.end(),
                [i = 0]() mutable { return Derived::headers[i++].size(); }
            );
        }

        bool empty() { return size() == 0; }

        void push_back(const T& source) {
            Derived::get_data(new_row(), source);
            resize();
        }

        void push_back(T&& source) {
            Derived::get_data(new_row(), std::move(source));
            resize();
        }

        size_t size() { return rows.size(); }

        friend std::ostream& operator<<(std::ostream& os, const table& t) {
            t.print(os);
            return os;
        }
    };
}
