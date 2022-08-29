// EditDistance.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <utility>

template<typename T> class CLevenshteinDistance
{
    private:
        T * m_pArray;
        size_t m_NoColumns;
        size_t m_NoRows;
        size_t m_MaxSize;
        size_t m_EditDistance;
        CLevenshteinDistance();

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
            std::cout << "Edit Distance is " << this->GetDistance() << std::endl;
        }
    public:
        bool IsValid() { return m_pArray != nullptr; }
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
        size_t GetDistance()const { return m_EditDistance; }
        size_t CalculateDistance(std::string a, std::string b)
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

        void PrintDistanceSequenze(std::string str1, std::string str2)
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

        CLevenshteinDistance(size_t columns, size_t rows): m_pArray(nullptr), m_NoColumns(columns), m_NoRows(rows), m_MaxSize(m_NoColumns * m_NoRows), m_EditDistance(0u)
        {
            const size_t size = m_MaxSize;
            m_pArray = new T[size];
            if (m_pArray == nullptr)
            {
                m_NoColumns = m_NoRows = m_MaxSize = 0u;
            }
        }

        CLevenshteinDistance(std::string str1, std::string str2): m_pArray(nullptr), m_NoColumns(str1.length()), m_NoRows(str2.length()), m_EditDistance(0u)
        {
            const size_t size = m_NoColumns * m_NoRows;
            m_MaxSize = size;
            m_pArray = new T[size];
            if (m_pArray != nullptr)
            {
                m_EditDistance = CalculateDistance(str1, str2);
            }
            else
            {
                m_NoColumns = m_NoRows = m_MaxSize = 0u;
            }
        }
        ~CLevenshteinDistance()
        {
            if (m_pArray != nullptr)
                delete[]m_pArray;
        }

};

int main()
{
    std::string str2 = "3";
    std::string str1 = "12345";
    CLevenshteinDistance<uint8_t> a(str1, str2);
    a.PrintDistanceSequenze(str1, str2);
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
