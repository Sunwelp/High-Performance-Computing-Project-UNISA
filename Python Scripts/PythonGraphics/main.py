# Course: High Performance Computing 2023/2024
#
# Lecturer: Francesco Moscato  fmoscato@unisa.it
#
# Student:
# Pepe Lorenzo        0622702121      l.pepe29@studenti.unisa.it          
# 
#
#                REQUIREMENTS OF THE ASSIGNMENT:
# Design, Implement, and Analyze a parallel version of VF2-pp based on MPI
#  
#  
# Copyright (C) 2024 - All Rights Reserved
#  
# This program is free software: you can redistribute it and/or modify it under the terms of 
# the GNU General Public License as published by the Free Software Foundation, either version 
# 3 of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
#  See the GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License along with ContestOMP. 
#  If not, see <http://www.gnu.org/licenses/>.

import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np

def createLinePlot():

    x = np.arange(10)
    y = np.arange(10)
    sns.lineplot(x=x, y=y, label="Reference", color="Red")

    x_custom = [2, 4, 8]
    y_custom = [1.042451, 0.730437, 0.487572]
    sns.lineplot(x=x_custom, y=y_custom, label="Speedup", color="blue", marker='o')

    plt.xlabel("# of processes")
    plt.ylabel("Speedup")
    plt.grid(True)
    plt.legend()
    plt.xlim(0,9)
    plt.ylim(0,9)

    plt.show()


if __name__ == '__main__':

	createLinePlot()


