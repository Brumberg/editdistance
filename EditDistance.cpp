// EditDistance.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <algorithm>
#include <unordered_map>

class CEditDistance
{
protected:
    CEditDistance() {}
    virtual ~CEditDistance() {}
    virtual size_t GetEditDistance()const = 0;
    virtual void PrintEditDistanceSequenze(std::string str1, std::string str2) = 0;
    virtual bool IsValid() = 0;
};

template<typename T> class CLevenshtein : public CEditDistance
{
    private:
        T * m_pArray;
        size_t m_NoColumns;
        size_t m_NoRows;
        size_t m_MaxSize;
        size_t m_EditDistance;
        CLevenshtein();

        void ShowSequence(std::string str1, std::string str2)
        {
            size_t row = m_NoRows-1u;
            size_t column = m_NoColumns - 1u;
            bool printsequence = row > 0 || column > 0;
            std::string result = str1;
            bool allow_substitution = true;
            bool firstrowreached = false;
            bool firstcolumnreached = false;
            
            size_t accum_cost = 0u;
            const size_t cost_insert = 1u;
            const size_t const_delete = 1u;
            const size_t substitude_alphanum = 1u;
            const size_t substitude_same = 0u;
            if ((m_NoRows > 0u) && (m_NoColumns > 0u))
            {
                while (printsequence)
                {
                    const T cvalue = m_pArray[row * m_NoColumns + column];
                    const T substitude = ((column > 0) && (row > 0)) ? m_pArray[(row - 1u) * m_NoColumns + (column - 1u)] : (column == 0u) ? static_cast<T>(row) : static_cast<T>(column);
                    const T op_del = (column > 0u) ? m_pArray[row * m_NoColumns + column - 1u] : static_cast<T>(row + 1u);
                    const T op_insert = (row > 0u) ? m_pArray[(row - 1u) * m_NoColumns + column] : static_cast<T>(column + 1u);

                    if ((substitude <= op_del) && (substitude <= op_insert) && (allow_substitution == true))
                    {
                        if (cvalue == substitude)
                        {
                            result[column] = str2[row];
                            accum_cost += substitude_same;
                            std::cout << "Substitution:\t" << "\t" << result << "\t" << "Cost: " << substitude_same << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                        }
                        else
                        {
                            result[column] = str2[row];
                            accum_cost += substitude_alphanum;
                            std::cout << "Substitution:\t" << "\t" << result << "\t" << "Cost: " << substitude_alphanum << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                        }
                        allow_substitution = !(column == 0 || row == 0);
                        firstrowreached = row == 0u;
                        firstcolumnreached = column == 0u;
                        row = row > 0u ? row - 1u : row;
                        column = column > 0u ? column - 1u : column;
                    }
                    else
                    {
                        if (op_del < op_insert)
                        {
                            result.erase(column, 1u);
                            accum_cost += const_delete;
                            std::cout << "Deletion:\t" << "\t" << result << "\t" << "Cost: " << const_delete << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                            allow_substitution = (column > 0) && (allow_substitution == true);
                            firstcolumnreached = column == 0u;
                            column = column > 0u ? column - 1u : column;
                        }
                        else
                        {
                            if (firstcolumnreached)
                            {
                                result.insert(column, 1u, str2[row]);
                            }
                            else
                            {
                                result.insert(column + 1u, 1u, str2[row]);
                            }
                            accum_cost += cost_insert;
                            std::cout << "Insertion:\t" << "\t" << result << "\t" << "Cost: " << cost_insert << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                            allow_substitution = (row > 0) && (allow_substitution == true);
                            firstrowreached = row == 0u;
                            row = row > 0u ? row - 1u : row;
                        }
                    }
                    printsequence = (row > 0u) || (column > 0u);
                }

                if (result.length() > 0u)
                {
                    if ((firstrowreached == true) && (firstcolumnreached == true) || (firstrowreached == false) && (firstcolumnreached == false))
                    {
                        if (result[0] == str2[0])
                        {
                            accum_cost += substitude_same;
                            std::cout << "Substitution:\t" << "\t" << result << "\t" << "Cost: " << substitude_same << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                        }
                        else
                        {
                            result[0] = str2[0];
                            accum_cost += substitude_alphanum;
                            std::cout << "Substitution:\t" << "\t" << result << "\t" << "Cost: " << substitude_alphanum << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                        }
                    }
                    else if (firstrowreached == true)
                    {
                        result.erase(column, 1u);
                        accum_cost += const_delete;
                        std::cout << "Deletion:\t" << "\t" << result << "\t" << "Cost: " << const_delete << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                    }
                    else
                    {
                        result.insert(0, 1u, str2[0]);
                        accum_cost += cost_insert;
                        std::cout << "Insertion:\t" << "\t" << result << "\t" << "Cost: " << cost_insert << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                    }
                    /*if (result.length() < str2.length())
                    {
                        result.insert(0, 1u, str2[0]);
                        std::cout << "Insertion" << "\t" << result << std::endl;
                    }
                    else if (result.length() > str2.length())
                    {
                        result.erase(column, 1u);
                        std::cout << "Deletion" << "\t" << result << std::endl;
                    }
                    else
                    {
                        if (result[0] == str2[0])
                        {
                            std::cout << "Substitution without cost" << "\t" << result << std::endl;
                        }
                        else
                        {
                            result[0] = str2[0];
                            std::cout << "Substitution with cost" << "\t" << result << std::endl;
                        }
                    }*/
                }
            }
            else
            {
                if ((m_NoRows == 0u) && (m_NoColumns == 0u))
                {
                    accum_cost = 0;
                }
                else if (m_NoColumns == 0u)
                {
                    size_t i = m_NoRows;
                    while (i != 0)
                    {
                        result.insert(0, 1u, str2[--i]);
                        accum_cost += cost_insert;
                        std::cout << "Insertion:\t" << "\t" << result << "\t" << "Cost: " << cost_insert << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                    }
                    
                }
                else
                {
                    size_t i = m_NoColumns;
                    while (i != 0)
                    {
                        result.erase(--i, 1u);
                        accum_cost += const_delete;
                        std::cout << "Deletion:\t" << "\t" << result << "\t" << "Cost: " << const_delete << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                    }
                }
            }
            std::cout << std::endl << std::endl;
            std::string status((result == str2) ? "Pass" : "Fail");
            std::cout << "Transforming: " << std::endl << "\t" << str1 << std::endl << " to " << std::endl << "\t" << result << std::endl << "in " << accum_cost << " " << "steps" << "." << std::endl;
            std::cout << "Test: \t" << status << std::endl;
            std::cout << "Edit Distance is " << this->GetEditDistance() << std::endl;
        }

        size_t CalculateEditDistance(std::string a, std::string b)
        {
            size_t distance = 0;
            if (m_pArray != nullptr)
            {
                if (a.length() > 0 && b.length() > 0)
                {
                    if ((a.length() <= m_NoColumns) && (b.length() <= m_NoRows))
                    {
                        T inc = static_cast<T>(1);
                        for (size_t i = 0u; i < m_NoColumns; ++i)
                        {
                            inc = (a[i] == b[0]) ? 0u : inc;
                            m_pArray[i] = static_cast<T>(i) + inc;
                        }

                        inc = static_cast<T>(1);
                        for (size_t i = 0u; i < m_NoRows; ++i)
                        {
                            inc = (a[0] == b[i]) ? 0u : inc;
                            m_pArray[i * m_NoColumns] = static_cast<T>(i) + inc;
                        }

                        size_t baserow = 0u;
                        for (size_t i = 1u; i < m_NoRows; ++i)
                        {
                            const size_t prevrow = baserow;
                            baserow += m_NoColumns;
                            const auto letter_b = b[i];
                            for (size_t j = 1u; j < m_NoColumns; ++j)
                            {
                                const T cost = (a[j] == letter_b) ? 0u : 1u;
                                const T rel_tail = m_pArray[prevrow + (j - 1)] + cost;
                                const T rel_indel = std::min(m_pArray[prevrow + j], m_pArray[baserow + (j - 1)]) + 1u;
                                const T overallmin = std::min(rel_tail, rel_indel);
                                m_pArray[baserow + j] = overallmin;
                            }
                        }
                        distance = static_cast<size_t>(m_pArray[m_NoColumns * m_NoRows - 1u]);
                    }
                }
                else
                {
                    distance = std::max(a.length(), b.length());
                }
            }
            return distance;
        }

        void Resize(size_t columns, size_t rows)
        {
            const size_t size = columns * rows;
            if (m_pArray != nullptr)
            {
                if (size <= m_MaxSize)
                {
                    m_NoColumns = columns;
                    m_NoRows = rows;

                }
                else
                {
                    T* p = new T[size];
                    if (p != nullptr)
                    {
                        std::swap(p, m_pArray);
                        delete[]p;
                        m_NoColumns = columns;
                        m_NoRows = rows;
                        m_MaxSize = size;
                    }
                }
            }
            else
            {
                m_pArray = new T[size];
                if (m_pArray == nullptr)
                {
                    m_NoColumns = m_NoRows = m_MaxSize = 0u;
                }
                else
                {
                    m_MaxSize = size;
                    m_NoColumns = columns;
                    m_NoRows = rows;
                }
            }
        }

    public:
        CLevenshtein(size_t columns, size_t rows) : CEditDistance(), m_pArray(nullptr), m_NoColumns(columns), m_NoRows(rows), m_MaxSize(m_NoColumns* m_NoRows), m_EditDistance(0u)
        {
            const size_t size = m_MaxSize;
            m_pArray = new T[size];
            if (m_pArray == nullptr)
            {
                m_NoColumns = m_NoRows = m_MaxSize = 0u;
            }
        }

        CLevenshtein(std::string str1, std::string str2) : CEditDistance(), m_pArray(nullptr), m_NoColumns(str1.length()), m_NoRows(str2.length()), m_EditDistance(0u)
        {
            const size_t size = m_NoColumns * m_NoRows;
            m_MaxSize = size;
            m_pArray = new T[size];
            if (m_pArray != nullptr)
            {
                m_EditDistance = CalculateEditDistance(str1, str2);
            }
            else
            {
                m_NoColumns = m_NoRows = m_MaxSize = 0u;
            }
        }

        virtual ~CLevenshtein()
        {
            if (m_pArray != nullptr)
                delete[]m_pArray;
        }

        virtual bool IsValid() { return m_pArray != nullptr; }
        virtual size_t GetEditDistance()const { return m_EditDistance; }
        virtual void PrintEditDistanceSequenze(std::string str1, std::string str2)
        {
            std::vector<std::string> todo;
            std::string original = str1;
            if ((m_EditDistance >= 0u) && (m_pArray != nullptr))
            {
                ShowSequence(str1, str2);
            }
            else
            {
                if (m_pArray == nullptr)
                {
                    std::cout << "Array is not initialized." << std::endl;
                }
                else
                {
                    std::cout << "Edit distance is zero." << std::endl;
                }
            }
        }
};

template<typename T> class COSALevenshtein : public CEditDistance
{
private:
    T* m_pArray;
    size_t m_NoColumns;
    size_t m_NoRows;
    size_t m_MaxSize;
    size_t m_EditDistance;
    COSALevenshtein();

    void ShowSequence(std::string str1, std::string str2)
    {
        size_t row = m_NoRows - 1u;
        size_t column = m_NoColumns - 1u;
        bool printsequence = row > 0 || column > 0;
        std::string result = str1;
        bool allow_substitution = true;
        bool allow_twist = (m_NoRows > 1u) && (m_NoColumns > 1u);
        bool firstrowreached = false;
        bool firstcolumnreached = false;
        bool finalswap = false;

        size_t accum_cost = 0u;
        const size_t cost_insert = 1u;
        const size_t const_delete = 1u;
        const size_t substitude_alphanum = 1u;
        const size_t cost_twist = 1u;
        const size_t substitude_same = 0u;
        if ((m_NoRows > 0u) && (m_NoColumns > 0u))
        {
            while (printsequence)
            {
                const T cvalue = m_pArray[row * m_NoColumns + column];
                const T substitude = ((column > 0u) && (row > 0u)) ? m_pArray[(row - 1u) * m_NoColumns + (column - 1u)] : (column == 0u) ? static_cast<T>(row) : static_cast<T>(column);
                const T op_del = (column > 0u) ? m_pArray[row * m_NoColumns + column - 1u] : static_cast<T>(row + 1u);
                const T op_insert = (row > 0u) ? m_pArray[(row - 1u) * m_NoColumns + column] : static_cast<T>(column + 1u);
                
                T twist = substitude;
                if (allow_twist == true)
                {
                    if (row > 0u && column > 0u)
                    {
                        if (str1[column] == str2[row - 1u] && str1[column - 1u] == str2[row])
                        {
                            if (row == 1u && column == 1u)
                            {
                                twist = 0u;
                            }
                            else
                            {
                                twist = m_pArray[(row - 2u) * m_NoColumns + (column - 2u)];
                            }
                        }
                    }
                    else
                    {
                        allow_twist = false;
                    }
                }

                if ((substitude <= op_del) && (substitude <= op_insert) && substitude <= twist && (allow_substitution == true))
                {
                    if (cvalue == substitude)
                    {
                        result[column] = str2[row];
                        accum_cost += substitude_same;
                        std::cout << "Substitution:\t" << "\t" << result << "\t" << "Cost: " << substitude_same << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                    }
                    else
                    {
                        result[column] = str2[row];
                        accum_cost += substitude_alphanum;
                        std::cout << "Substitution:\t" << "\t" << result << "\t" << "Cost: " << substitude_alphanum << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                    }
                    allow_substitution = !(column == 0 || row == 0);
                    firstrowreached = row == 0u;
                    firstcolumnreached = column == 0u;
                    row = row > 0u ? row - 1u : row;
                    column = column > 0u ? column - 1u : column;
                }
                else if ((twist <= op_del) && (twist <= op_insert) && (allow_twist == true))
                {
                    result[column] = str2[row];
                    result[column-1u] = str2[row-1u];
                    accum_cost += cost_twist;
                    std::cout << "Twist:\t" << "\t" << result << "\t" << "Cost: " << cost_twist << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                    row = row > 1u ? row - 2u : 0;
                    column = column > 1u ? column - 2u : 0;
                    allow_twist = row > 0u && column > 0u;
                    finalswap = column==0 && row==0;
                }
                else
                {
                    if (op_del < op_insert)
                    {
                        result.erase(column, 1u);
                        accum_cost += const_delete;
                        std::cout << "Deletion:\t" << "\t" << result << "\t" << "Cost: " << const_delete << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                        allow_substitution = (column > 0) && (allow_substitution == true);
                        firstcolumnreached = column == 0u;
                        column = column > 0u ? column - 1u : column;
                    }
                    else
                    {
                        if (firstcolumnreached)
                        {
                            result.insert(column, 1u, str2[row]);
                        }
                        else
                        {
                            result.insert(column + 1u, 1u, str2[row]);
                        }
                        accum_cost += cost_insert;
                        std::cout << "Insertion:\t" << "\t" << result << "\t" << "Cost: " << cost_insert << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                        allow_substitution = (row > 0) && (allow_substitution == true);
                        firstrowreached = row == 0u;
                        row = row > 0u ? row - 1u : row;
                    }
                }
                printsequence = (row > 0u) || (column > 0u);
            }

            if (result.length() > 0u)
            {
                if (finalswap == false)
                {
                    if ((firstrowreached == true) && (firstcolumnreached == true) || (firstrowreached == false) && (firstcolumnreached == false))
                    {
                        if (result[0] == str2[0])
                        {
                            accum_cost += substitude_same;
                            std::cout << "Substitution:\t" << "\t" << result << "\t" << "Cost: " << substitude_same << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                        }
                        else
                        {
                            result[0] = str2[0];
                            accum_cost += substitude_alphanum;
                            std::cout << "Substitution:\t" << "\t" << result << "\t" << "Cost: " << substitude_alphanum << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                        }
                    }
                    else if (firstrowreached == true)
                    {
                        result.erase(column, 1u);
                        accum_cost += const_delete;
                        std::cout << "Deletion:\t" << "\t" << result << "\t" << "Cost: " << const_delete << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                    }
                    else
                    {
                        result.insert(0, 1u, str2[0]);
                        accum_cost += cost_insert;
                        std::cout << "Insertion:\t" << "\t" << result << "\t" << "Cost: " << cost_insert << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                    }
                    /*if (result.length() < str2.length())
                    {
                        result.insert(0, 1u, str2[0]);
                        std::cout << "Insertion" << "\t" << result << std::endl;
                    }
                    else if (result.length() > str2.length())
                    {
                        result.erase(column, 1u);
                        std::cout << "Deletion" << "\t" << result << std::endl;
                    }
                    else
                    {
                        if (result[0] == str2[0])
                        {
                            std::cout << "Substitution without cost" << "\t" << result << std::endl;
                        }
                        else
                        {
                            result[0] = str2[0];
                            std::cout << "Substitution with cost" << "\t" << result << std::endl;
                        }
                    }*/
                }
            }
        }
        else
        {
            if ((m_NoRows == 0u) && (m_NoColumns == 0u))
            {
                accum_cost = 0;
            }
            else if (m_NoColumns == 0u)
            {
                size_t i = m_NoRows;
                while (i != 0)
                {
                    result.insert(0, 1u, str2[--i]);
                    accum_cost += cost_insert;
                    std::cout << "Insertion:\t" << "\t" << result << "\t" << "Cost: " << cost_insert << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                }

            }
            else
            {
                size_t i = m_NoColumns;
                while (i != 0)
                {
                    result.erase(--i, 1u);
                    accum_cost += const_delete;
                    std::cout << "Deletion:\t" << "\t" << result << "\t" << "Cost: " << const_delete << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                }
            }
        }
        std::cout << std::endl << std::endl;
        std::string status((result == str2) ? "Pass" : "Fail");
        std::cout << "Transforming: " << std::endl << "\t" << str1 << std::endl << " to " << std::endl << "\t" << result << std::endl << "in " << accum_cost << " " << "steps" << "." << std::endl;
        std::cout << "Test: \t" << status << std::endl;
        std::cout << "Edit Distance is " << this->GetEditDistance() << std::endl;
    }

    size_t CalculateEditDistance(std::string a, std::string b)
    {
        size_t distance = 0;
        if (m_pArray != nullptr)
        {
            if (a.length() > 0 && b.length() > 0)
            {
                if ((a.length() <= m_NoColumns) && (b.length() <= m_NoRows))
                {
                    T inc = static_cast<T>(1);
                    for (size_t i = 0u; i < m_NoColumns; ++i)
                    {
                        inc = (a[i] == b[0]) ? 0u : inc;
                        m_pArray[i] = static_cast<T>(i) + inc;
                    }

                    inc = static_cast<T>(1);
                    for (size_t i = 0u; i < m_NoRows; ++i)
                    {
                        inc = (a[0] == b[i]) ? 0u : inc;
                        m_pArray[i * m_NoColumns] = static_cast<T>(i) + inc;
                    }

                    size_t baserow = 0u;
                    if (1u < m_NoRows)
                    {
                        const size_t prevrow = baserow;
                        baserow += m_NoColumns;
                        const auto letter_b = b[1];
                        for (size_t j = 1u; j < m_NoColumns; ++j)
                        {
                            const T cost = (a[j] == letter_b) ? 0u : 1u;
                            const T rel_tail = m_pArray[prevrow + (j - 1)] + cost;
                            const T rel_indel = std::min(m_pArray[prevrow + j], m_pArray[baserow + (j - 1)]) + 1u;
                            const T overallmin = std::min(rel_tail, rel_indel);
                            m_pArray[baserow + j] = overallmin;
                        }
                    }

                    
                    if (1u < m_NoColumns)
                    {
                        baserow = 0;
                        for (size_t i = 1u; i < m_NoRows; ++i)
                        {
                            const size_t prevrow = baserow;
                            baserow += m_NoColumns;
                            const auto letter_b = b[i];
                            
                            const T cost = (a[1u] == letter_b) ? 0u : 1u;
                            const T rel_tail = m_pArray[prevrow] + cost;
                            const T rel_indel = std::min(m_pArray[prevrow + 1], m_pArray[baserow]) + 1u;
                            const T overallmin = std::min(rel_tail, rel_indel);
                            m_pArray[baserow + 1] = overallmin;
                        }
                    }

                    if (1u < m_NoRows && 1u < m_NoColumns)
                    {
                        const T edit_cost = m_pArray[baserow + 1u];
                        const auto letter_b_prev = b[0];
                        const auto letter_a_prev = a[0];
                        const auto letter_b = b[1];
                        const auto letter_a = a[1];
                        if (letter_b_prev == letter_a && letter_a_prev == letter_b)
                        {
                            m_pArray[baserow + 1u] = std::min(edit_cost, static_cast<T>(1u));
                        }
                    }

                    for (size_t i = 2u; i < m_NoRows; ++i)
                    {
                        const size_t pprevrow = (i - 2u) * m_NoColumns;
                        const size_t prevrow = (i - 1u) * m_NoColumns;
                        const size_t baserow = i * m_NoColumns;
                        const auto letter_b = b[i];
                        const auto letter_b_twist = b[i - 1u];
                        for (size_t j = 2u; j < m_NoColumns; ++j)
                        {
                            const auto letter_a_twist = a[j - 1u];
                            const T cost = (a[j] == letter_b) ? 0u : 1u;
                            const T rel_tail = m_pArray[prevrow + (j - 1u)] + cost;
                            const T rel_indel = std::min(m_pArray[prevrow + j], m_pArray[baserow + (j - 1u)]) + 1u;
                            const T overallmin = std::min(rel_tail, rel_indel);
                            if (letter_b_twist == a[j] && letter_a_twist == letter_b)
                            {
                                const T rel_twist = (m_pArray[pprevrow + (j - 2u)], m_pArray[pprevrow + (j - 2u)]) + 1u;
                                m_pArray[baserow + j] = std::min(overallmin, rel_twist);
                            }
                            else
                            {
                                m_pArray[baserow + j] = overallmin;
                            }
                        }
                    }

                    distance = static_cast<size_t>(m_pArray[m_NoColumns * m_NoRows - 1u]);
                }
            }
            else
            {
                distance = std::max(a.length(), b.length());
            }
        }
        return distance;
    }

    void Resize(size_t columns, size_t rows)
    {
        const size_t size = columns * rows;
        if (m_pArray != nullptr)
        {
            if (size <= m_MaxSize)
            {
                m_NoColumns = columns;
                m_NoRows = rows;

            }
            else
            {
                T* p = new T[size];
                if (p != nullptr)
                {
                    std::swap(p, m_pArray);
                    delete[]p;
                    m_NoColumns = columns;
                    m_NoRows = rows;
                    m_MaxSize = size;
                }
            }
        }
        else
        {
            m_pArray = new T[size];
            if (m_pArray == nullptr)
            {
                m_NoColumns = m_NoRows = m_MaxSize = 0u;
            }
            else
            {
                m_MaxSize = size;
                m_NoColumns = columns;
                m_NoRows = rows;
            }
        }
    }

public:
    COSALevenshtein(size_t columns, size_t rows) : CEditDistance(), m_pArray(nullptr), m_NoColumns(columns), m_NoRows(rows), m_MaxSize(m_NoColumns* m_NoRows), m_EditDistance(0u)
    {
        const size_t size = m_MaxSize;
        m_pArray = new T[size];
        if (m_pArray == nullptr)
        {
            m_NoColumns = m_NoRows = m_MaxSize = 0u;
        }
    }

    COSALevenshtein(std::string str1, std::string str2) : CEditDistance(), m_pArray(nullptr), m_NoColumns(str1.length()), m_NoRows(str2.length()), m_EditDistance(0u)
    {
        const size_t size = m_NoColumns * m_NoRows;
        m_MaxSize = size;
        m_pArray = new T[size];
        if (m_pArray != nullptr)
        {
            m_EditDistance = CalculateEditDistance(str1, str2);
        }
        else
        {
            m_NoColumns = m_NoRows = m_MaxSize = 0u;
        }
    }

    virtual ~COSALevenshtein()
    {
        if (m_pArray != nullptr)
            delete[]m_pArray;
    }

    virtual bool IsValid() { return m_pArray != nullptr; }
    virtual size_t GetEditDistance()const { return m_EditDistance; }
    virtual void PrintEditDistanceSequenze(std::string str1, std::string str2)
    {
        std::vector<std::string> todo;
        std::string original = str1;
        if ((m_EditDistance >= 0u) && (m_pArray != nullptr))
        {
            ShowSequence(str1, str2);
        }
        else
        {
            if (m_pArray == nullptr)
            {
                std::cout << "Array is not initialized." << std::endl;
            }
            else
            {
                std::cout << "Edit distance is zero." << std::endl;
            }
        }
    }
};

template<typename T> class CDamerauLevenshtein : public CEditDistance
{
private:
    T* m_pArray;
    size_t m_NoColumns;
    size_t m_NoRows;
    size_t m_MaxSize;
    size_t m_EditDistance;
    CDamerauLevenshtein();

    void ShowSequence(std::string str1, std::string str2)
    {
        size_t row = m_NoRows - 1u;
        size_t column = m_NoColumns - 1u;
        bool printsequence = row > 0 || column > 0;
        std::string result = str1;
        bool allow_substitution = true;
        bool allow_twist = (m_NoRows > 1u) && (m_NoColumns > 1u);
        bool firstrowreached = false;
        bool firstcolumnreached = false;
        bool finalswap = false;

        size_t accum_cost = 0u;
        const size_t cost_insert = 1u;
        const size_t const_delete = 1u;
        const size_t substitude_alphanum = 1u;
        const size_t cost_twist = 1u;
        const size_t substitude_same = 0u;
        if ((m_NoRows > 0u) && (m_NoColumns > 0u))
        {
            while (printsequence)
            {
                const T cvalue = m_pArray[row * m_NoColumns + column];
                const T substitude = ((column > 0u) && (row > 0u)) ? m_pArray[(row - 1u) * m_NoColumns + (column - 1u)] : (column == 0u) ? static_cast<T>(row) : static_cast<T>(column);
                const T op_del = (column > 0u) ? m_pArray[row * m_NoColumns + column - 1u] : static_cast<T>(row + 1u);
                const T op_insert = (row > 0u) ? m_pArray[(row - 1u) * m_NoColumns + column] : static_cast<T>(column + 1u);

                T twist = substitude;
                if (allow_twist == true)
                {
                    if (row > 0u && column > 0u)
                    {
                        if (str1[column] == str2[row - 1u] && str1[column - 1u] == str2[row])
                        {
                            if (row == 1u && column == 1u)
                            {
                                twist = 0u;
                            }
                            else
                            {
                                twist = m_pArray[(row - 2u) * m_NoColumns + (column - 2u)];
                            }
                        }
                    }
                    else
                    {
                        allow_twist = false;
                    }
                }

                if ((substitude <= op_del) && (substitude <= op_insert) && substitude <= twist && (allow_substitution == true))
                {
                    if (cvalue == substitude)
                    {
                        result[column] = str2[row];
                        accum_cost += substitude_same;
                        std::cout << "Substitution:\t" << "\t" << result << "\t" << "Cost: " << substitude_same << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                    }
                    else
                    {
                        result[column] = str2[row];
                        accum_cost += substitude_alphanum;
                        std::cout << "Substitution:\t" << "\t" << result << "\t" << "Cost: " << substitude_alphanum << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                    }
                    allow_substitution = !(column == 0 || row == 0);
                    firstrowreached = row == 0u;
                    firstcolumnreached = column == 0u;
                    row = row > 0u ? row - 1u : row;
                    column = column > 0u ? column - 1u : column;
                }
                else if ((twist <= op_del) && (twist <= op_insert) && (allow_twist == true))
                {
                    result[column] = str2[row];
                    result[column - 1u] = str2[row - 1u];
                    accum_cost += cost_twist;
                    std::cout << "Twist:\t" << "\t" << result << "\t" << "Cost: " << cost_twist << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                    row = row > 1u ? row - 2u : 0;
                    column = column > 1u ? column - 2u : 0;
                    allow_twist = row > 0u && column > 0u;
                    finalswap = column == 0 && row == 0;
                }
                else
                {
                    if (op_del < op_insert)
                    {
                        result.erase(column, 1u);
                        accum_cost += const_delete;
                        std::cout << "Deletion:\t" << "\t" << result << "\t" << "Cost: " << const_delete << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                        allow_substitution = (column > 0) && (allow_substitution == true);
                        firstcolumnreached = column == 0u;
                        column = column > 0u ? column - 1u : column;
                    }
                    else
                    {
                        if (firstcolumnreached)
                        {
                            result.insert(column, 1u, str2[row]);
                        }
                        else
                        {
                            result.insert(column + 1u, 1u, str2[row]);
                        }
                        accum_cost += cost_insert;
                        std::cout << "Insertion:\t" << "\t" << result << "\t" << "Cost: " << cost_insert << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                        allow_substitution = (row > 0) && (allow_substitution == true);
                        firstrowreached = row == 0u;
                        row = row > 0u ? row - 1u : row;
                    }
                }
                printsequence = (row > 0u) || (column > 0u);
            }

            if (result.length() > 0u)
            {
                if (finalswap == false)
                {
                    if ((firstrowreached == true) && (firstcolumnreached == true) || (firstrowreached == false) && (firstcolumnreached == false))
                    {
                        if (result[0] == str2[0])
                        {
                            accum_cost += substitude_same;
                            std::cout << "Substitution:\t" << "\t" << result << "\t" << "Cost: " << substitude_same << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                        }
                        else
                        {
                            result[0] = str2[0];
                            accum_cost += substitude_alphanum;
                            std::cout << "Substitution:\t" << "\t" << result << "\t" << "Cost: " << substitude_alphanum << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                        }
                    }
                    else if (firstrowreached == true)
                    {
                        result.erase(column, 1u);
                        accum_cost += const_delete;
                        std::cout << "Deletion:\t" << "\t" << result << "\t" << "Cost: " << const_delete << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                    }
                    else
                    {
                        result.insert(0, 1u, str2[0]);
                        accum_cost += cost_insert;
                        std::cout << "Insertion:\t" << "\t" << result << "\t" << "Cost: " << cost_insert << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                    }
                    /*if (result.length() < str2.length())
                    {
                        result.insert(0, 1u, str2[0]);
                        std::cout << "Insertion" << "\t" << result << std::endl;
                    }
                    else if (result.length() > str2.length())
                    {
                        result.erase(column, 1u);
                        std::cout << "Deletion" << "\t" << result << std::endl;
                    }
                    else
                    {
                        if (result[0] == str2[0])
                        {
                            std::cout << "Substitution without cost" << "\t" << result << std::endl;
                        }
                        else
                        {
                            result[0] = str2[0];
                            std::cout << "Substitution with cost" << "\t" << result << std::endl;
                        }
                    }*/
                }
            }
        }
        else
        {
            if ((m_NoRows == 0u) && (m_NoColumns == 0u))
            {
                accum_cost = 0;
            }
            else if (m_NoColumns == 0u)
            {
                size_t i = m_NoRows;
                while (i != 0)
                {
                    result.insert(0, 1u, str2[--i]);
                    accum_cost += cost_insert;
                    std::cout << "Insertion:\t" << "\t" << result << "\t" << "Cost: " << cost_insert << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                }

            }
            else
            {
                size_t i = m_NoColumns;
                while (i != 0)
                {
                    result.erase(--i, 1u);
                    accum_cost += const_delete;
                    std::cout << "Deletion:\t" << "\t" << result << "\t" << "Cost: " << const_delete << "\t " << "Accumulated cost:" << accum_cost << std::endl;
                }
            }
        }
        std::cout << std::endl << std::endl;
        std::string status((result == str2) ? "Pass" : "Fail");
        std::cout << "Transforming: " << std::endl << "\t" << str1 << std::endl << " to " << std::endl << "\t" << result << std::endl << "in " << accum_cost << " " << "steps" << "." << std::endl;
        std::cout << "Test: \t" << status << std::endl;
        std::cout << "Edit Distance is " << this->GetEditDistance() << std::endl;
    }

    size_t CalculateEditDistance(std::string a, std::string b)
    {
        size_t distance = 0;
        if (m_pArray != nullptr)
        {
            if (a.length() > 0 && b.length() > 0)
            {
                if ((a.length() <= m_NoColumns) && (b.length() <= m_NoRows))
                {
                    T inc = static_cast<T>(1);
                    for (size_t i = 0u; i < m_NoColumns; ++i)
                    {
                        inc = (a[i] == b[0]) ? 0u : inc;
                        m_pArray[i] = static_cast<T>(i) + inc;
                    }

                    inc = static_cast<T>(1);
                    for (size_t i = 0u; i < m_NoRows; ++i)
                    {
                        inc = (a[0] == b[i]) ? 0u : inc;
                        m_pArray[i * m_NoColumns] = static_cast<T>(i) + inc;
                    }

                    size_t baserow = 0u;
                    if (1u < m_NoRows)
                    {
                        const size_t prevrow = baserow;
                        baserow += m_NoColumns;
                        const auto letter_b0 = b[0];
                        const auto letter_b1 = b[1];
                        
                        std::unordered_map<std::string::value_type, T> letters_of_a;
                        letters_of_a[a[0]] = 0;

                        for (size_t j = 1u; j < m_NoColumns; ++j)
                        {
                            const T cost = (a[j] == letter_b1) ? 0u : 1u;
                            const T rel_tail = m_pArray[prevrow + (j - 1)] + cost;
                            const T rel_indel = std::min(m_pArray[prevrow + j], m_pArray[baserow + (j - 1)]) + 1u;
                            const T overallmin = std::min(rel_tail, rel_indel);
                            if ((a[j] == letter_b0) && letters_of_a.count(letter_b1))
                            {
                                const T columnnumber = letters_of_a[letter_b1];
                                const T basecost = (columnnumber > 0) ? m_pArray[columnnumber - 1]:0;
                                const T deletions = static_cast<T>(j) - letters_of_a[letter_b1] - 1;
                                const T rel_twist = 1 + deletions + basecost;
                                m_pArray[baserow + j] = std::min(overallmin, rel_twist);
                            }
                            else
                            {
                                m_pArray[baserow + j] = overallmin;
                            }
                            letters_of_a[a[j]] = static_cast<T>(j);
                        }
                    }

                    if (1u < m_NoColumns)
                    {
                        baserow = 0;
                        std::unordered_map<std::string::value_type, T> letters_of_b;
                        letters_of_b[b[0]] = 0;

                        for (size_t i = 1u; i < m_NoRows; ++i)
                        {
                            const size_t prevrow = baserow;
                            baserow += m_NoColumns;
                            const auto letter_b = b[i];

                            const T cost = (a[1u] == letter_b) ? 0u : 1u;
                            const T rel_tail = m_pArray[prevrow] + cost;
                            const T rel_indel = std::min(m_pArray[prevrow + 1], m_pArray[baserow]) + 1u;
                            const T overallmin = std::min(rel_tail, rel_indel);
                            if (letter_b == a[0] && letters_of_b.count(a[1]))
                            {
                                const T rowno = letters_of_b[a[1]];
                                const T basecost = (rowno < m_NoColumns)? 0: m_pArray[m_NoColumns * (rowno - 1)];
                                const T insertions = static_cast<T>(i) - letters_of_b[a[1]] - 1;
                                const T rel_twist = 1 + insertions + basecost;
                                m_pArray[baserow + 1] = std::min(overallmin, rel_twist);
                            }
                            else
                            {
                                m_pArray[baserow + 1] = overallmin;
                            }
                            letters_of_b[b[i]] = static_cast<T>(i);
                        }
                    }

                    std::unordered_map<std::string::value_type, T> letters_of_b;
                    std::unordered_map<std::string::value_type, T> letters_of_a;
                    if (1u < m_NoRows && 1u < m_NoColumns)
                    {
#if 0
                        const T edit_cost = m_pArray[baserow + 1u];
                        const auto letter_b_prev = b[0];
                        const auto letter_a_prev = a[0];
                        const auto letter_b = b[1];
                        const auto letter_a = a[1];
                        if (letter_b_prev == letter_a && letter_a_prev == letter_b)
                        {
                            m_pArray[baserow + 1u] = std::min(edit_cost, static_cast<T>(1u));
                        }
                        else
                        {
                            letters_of_b[b[0]] = 0;
                            letters_of_b[b[1]] = 1;
                            letters_of_a[a[0]] = 0;
                            letters_of_a[a[1]] = 1;
                        }
#endif
                        letters_of_b[b[0]] = 0;
                        letters_of_b[b[1]] = 1;
                        letters_of_a[a[0]] = 0;
                        letters_of_a[a[1]] = 1;
                    }

                    for (size_t i = 2u; i < m_NoRows; ++i)
                    {
                        const size_t prevrow = (i - 1u) * m_NoColumns;
                        const size_t baserow = i * m_NoColumns;
                        const auto letter_b = b[i];
                        std::unordered_map<std::string::value_type, T> init_letters_of_a = letters_of_a;
                        for (size_t j = 2u; j < m_NoColumns; ++j)
                        {
                            const T cost = (a[j] == letter_b) ? 0u : 1u;
                            const T rel_tail = m_pArray[prevrow + (j - 1u)] + cost;//substitution
                            const T rel_indel = std::min(m_pArray[prevrow + j], m_pArray[baserow + (j - 1u)]) + 1u;//insertion or deletion
                            const T overallmin = std::min(rel_tail, rel_indel);
                            if (letters_of_b.count(a[j]) && init_letters_of_a.count(letter_b))
                            {
                                const T letter_a_of_b = init_letters_of_a[letter_b];
                                const T letter_b_of_a = letters_of_b[a[j]];
                                const T basecost = (letter_a_of_b == 0) ? (letter_b_of_a == 0)? 0 : letter_b_of_a 
                                    : (letter_b_of_a == 0) ? letter_a_of_b:m_pArray[letter_a_of_b - 1 + m_NoColumns * (letter_b_of_a - 1)];
                                const T deletions = static_cast<T>(j) - init_letters_of_a[letter_b] - 1;
                                const T insertions = static_cast<T>(i) - letter_b_of_a - 1;
                                const T rel_twist = 1 + deletions + insertions + basecost;
                                m_pArray[baserow + j] = std::min(overallmin, rel_twist);
                            }
                            else
                            {
                                m_pArray[baserow + j] = overallmin;
                            }
                            init_letters_of_a[a[j]] = static_cast<T>(j);
                        }
                        letters_of_b[b[i]] = static_cast<T>(i);
                    }

                    distance = static_cast<size_t>(m_pArray[m_NoColumns * m_NoRows - 1u]);
                }
            }
            else
            {
                distance = std::max(a.length(), b.length());
            }
        }
        return distance;
    }

    void Resize(size_t columns, size_t rows)
    {
        const size_t size = columns * rows;
        if (m_pArray != nullptr)
        {
            if (size <= m_MaxSize)
            {
                m_NoColumns = columns;
                m_NoRows = rows;

            }
            else
            {
                T* p = new T[size];
                if (p != nullptr)
                {
                    std::swap(p, m_pArray);
                    delete[]p;
                    m_NoColumns = columns;
                    m_NoRows = rows;
                    m_MaxSize = size;
                }
            }
        }
        else
        {
            m_pArray = new T[size];
            if (m_pArray == nullptr)
            {
                m_NoColumns = m_NoRows = m_MaxSize = 0u;
            }
            else
            {
                m_MaxSize = size;
                m_NoColumns = columns;
                m_NoRows = rows;
            }
        }
    }

public:
    CDamerauLevenshtein(size_t columns, size_t rows) : CEditDistance(), m_pArray(nullptr), m_NoColumns(columns), m_NoRows(rows), m_MaxSize(m_NoColumns* m_NoRows), m_EditDistance(0u)
    {
        const size_t size = m_MaxSize;
        m_pArray = new T[size];
        if (m_pArray == nullptr)
        {
            m_NoColumns = m_NoRows = m_MaxSize = 0u;
        }
    }

    CDamerauLevenshtein(std::string str1, std::string str2) : CEditDistance(), m_pArray(nullptr), m_NoColumns(str1.length()), m_NoRows(str2.length()), m_EditDistance(0u)
    {
        const size_t size = m_NoColumns * m_NoRows;
        m_MaxSize = size;
        m_pArray = new T[size];
        if (m_pArray != nullptr)
        {
            m_EditDistance = CalculateEditDistance(str1, str2);
        }
        else
        {
            m_NoColumns = m_NoRows = m_MaxSize = 0u;
        }
    }

    virtual ~CDamerauLevenshtein()
    {
        if (m_pArray != nullptr)
            delete[]m_pArray;
    }

    virtual bool IsValid() { return m_pArray != nullptr; }
    virtual size_t GetEditDistance()const { return m_EditDistance; }
    virtual void PrintEditDistanceSequenze(std::string str1, std::string str2)
    {
        std::vector<std::string> todo;
        std::string original = str1;
        if ((m_EditDistance >= 0u) && (m_pArray != nullptr))
        {
            ShowSequence(str1, str2);
        }
        else
        {
            if (m_pArray == nullptr)
            {
                std::cout << "Array is not initialized." << std::endl;
            }
            else
            {
                std::cout << "Edit distance is zero." << std::endl;
            }
        }
    }
};

int main()
{
    //std::string str1 = "tomcat";
    //std::string str2 = "mttaco";
    std::string str1 = "ka";
    std::string str2 = "akb";
    CDamerauLevenshtein<uint8_t> a(str1, str2);
    a.PrintEditDistanceSequenze(str1, str2);
}

// Programm ausführen: STRG+F5 oder Menüeintrag "Debuggen" > "Starten ohne Debuggen starten"
// Programm debuggen: F5 oder "Debuggen" > Menü "Debuggen starten"

// Tipps für den Einstieg: 
//   1. Verwenden Sie das Projektmappen-Explorer-Fenster zum Hinzufügen/Verwalten von Dateien.
//   2. Verwenden Sie das Team Explorer-Fenster zum Herstellen einer Verbindung mit der Quellcodeverwaltung.
//   3. Verwenden Sie das Ausgabefenster, um die Buildausgabe und andere Nachrichten anzuzeigen.
//   4. Verwenden Sie das Fenster "Fehlerliste", um Fehler anzuzeigen.
//   5. Wechseln Sie zu "Projekt" > "Neues Element hinzufügen", um neue Codedateien zu erstellen, bzw. zu "Projekt" > "Vorhandenes Element hinzufügen", um dem Projekt vorhandene Codedateien hinzuzufügen.
//   6. Um dieses Projekt später erneut zu öffnen, wechseln Sie zu "Datei" > "Öffnen" > "Projekt", und wählen Sie die SLN-Datei aus.
