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

#ifndef VOISIN_H
#define VOISIN_H

#include <iostream>

class Voisin
{
public:
    Voisin();
    virtual ~Voisin(){}
    /** Getter and setter */
    int getGain() const { return gain; }
    void setGain(int x) { gain = x; }
    
    /** Display */
    virtual std::ostream& print( std::ostream& out) = 0;
    friend std::ostream& operator<<(std::ostream& out, Voisin& v){
	return v.print(out);
    }
    
protected:
    int gain;
};

#endif // VOISIN_H
