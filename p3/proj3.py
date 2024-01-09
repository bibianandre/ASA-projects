from pulp import *

t, p, max_prod = map(int, sys.stdin.readline().split())
pack_per_toy = {i: [] for i in range(0,t)}
toys = [tuple(map(int, sys.stdin.readline().split())) for _ in range(t)]
packs = []
packs_constraint = []

for i in range (0,p):
    i1, i2, i3, profit = map(int, sys.stdin.readline().split())
    packs.append(profit)
    pack_per_toy[i1-1].append(i) 
    pack_per_toy[i2-1].append(i)
    pack_per_toy[i3-1].append(i)
    packs_constraint.append(min([toys[i1-1][1],toys[i2-1][1],toys[i3-1][1]]))

#Create lists of variables
vars1 = []
for i in range(t):
    vars1.append(LpVariable(f"X{i}", lowBound=0, upBound=toys[i][1],cat=LpInteger))

vars2 = []
for i in range(p):
    vars2.append(LpVariable(f"Y{i}", lowBound=0, upBound=packs_constraint[i], cat=LpInteger))

#Create problem
prob = LpProblem("UbiquityInc",LpMaximize)

#Create objective function
prob += (lpSum(vars1[v]*toys[v][0] for v in range(t))+lpSum(vars2[i]*packs[i] for i in range(p)))

#The general production limit
prob += (lpSum(vars1[v] for v in range(t)) + lpSum(3*vars2[i] for i in range(p))) <= max_prod

#Constraints for toys
for i, lim in enumerate(toys):
    if pack_per_toy[i]:
        prob += (vars1[i] + lpSum(vars2[j] for j in pack_per_toy[i])) <= lim[1]

prob.solve(GLPK_CMD(msg=0))

print(value(prob.objective)) 