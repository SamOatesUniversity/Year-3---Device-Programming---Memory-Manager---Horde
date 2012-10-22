/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SOGITYPES_HEADER
#define SOGITYPES_HEADER

#include "SOGI.h"
#include <string.h>

#define EX 0
#define EY 1
#define EZ 2
#define EW 3

template<typename T, unsigned int size>
class TVector {
private:
    T                   m_values[size];                 //!< The elementes of the vector
    unsigned int        m_size;                         //!< The size of the vector
public:
                        //! Class constructor
                        TVector()
                        {
                            m_size = size;
                        }

                        //! Get the x element of the vector
    T                   x() const
                        {
                            //ASSERT(this->m_size >= (EX+1), "Trying to access vector element greater than the size of the vector");
                            return m_values[EX];
                        }

                        //! Get the y element of the vector
    T                   y() const
                        {
                            //ASSERT(this->m_size >= (EY+1), "Trying to access vector element greater than the size of the vector");
                            return m_values[EY];
                        }

                        //! Get the z element of the vector
    T                   z() const
                        {
                            //ASSERT(this->m_size >= (EZ+1), "Trying to access vector element greater than the size of the vector");
                            return m_values[EZ];
                        }

                        //! Get the w element of the vector
    T                   w() const
                        {
                            //ASSERT(this->m_size >= (EW+1), "Trying to access vector element greater than the size of the vector");
                            return m_values[EW];
                        }

                        //! Equals operator
    TVector             &operator=(const TVector &rhs)
                        {
                            memcpy(this, &rhs, sizeof(TVector));
                            return *this;
                        }

                        //!
    void                Set(
                            const T x,
                            const T y = 0,
                            const T z = 0,
                            const T w = 0
                        )
                        {
                            if (this->m_size >= (EX+1)) m_values[EX] = x;
                            if (this->m_size >= (EY+1)) m_values[EY] = y;
                            if (this->m_size >= (EZ+1)) m_values[EZ] = z;
                            if (this->m_size >= (EW+1)) m_values[EW] = w;
                        }

                        //!
    void                SetX(
                            const T x
                        )
                        {
                            m_values[EX] = x;
                        }

                        //!
    void                SetY(
                            const T y
                        )
                        {
                            m_values[EY] = y;
                        }

                        //!
    void                SetZ(
                            const T z
                        )
                        {
                            m_values[EZ] = z;
                        }

                        //!
    void                SetW(
                            const T w
                        )
                        {
                            m_values[EW] = w;
                        }
};

#endif
