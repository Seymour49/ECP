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

#ifndef ONEMOVE_H
#define ONEMOVE_H

#include "voisin.h"

class OneMove : public Voisin
{
public:
    OneMove(int vertex, int ci, int cj);
    
    /** Getters and setter */
    int getS() const { return s; }
    int getVki() const { return vki; }
    int getVkj() const { return vkj; }

    /** Display */
    std::ostream& print(std::ostream& out);
    
private:
    int s;
    int vki;
    int vkj;
};

#endif // ONEMOVE_H
