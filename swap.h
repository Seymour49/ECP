/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2016  <copyright holder> <email>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef SWAP_H
#define SWAP_H

#include "voisin.h"

class Swap : public Voisin
{
public:
    Swap(int i, int j);
    /** Getters and setter */
    int getSi() const { return si; }
    int getSj() const { return sj; }
    
    /** Display */
    std::ostream& print(std::ostream& out);
    
private:
    int si;
    int sj;
};

#endif // SWAP_H
