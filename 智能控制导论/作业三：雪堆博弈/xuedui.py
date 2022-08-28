import random

# r<1/kmax
r = 0.1

# 收益矩阵
rewardMat = {
    'C': {'C': (1, 1), 'D': (1 - r, 1 + r)},
    'D': {'C': (1 + r, 1 - r), 'D': (0, 0)}
}


# 节点类，每个节点保存节点状态，邻居节点列表和邻居节点个数
class Node:
    def __init__(self):
        if random.random() < 0.5:
            self.state = 'C'
        else:
            self.state = 'D'
        self.value = 0
        self.all_value = 0
        self.neighbour_number = 0
        self.nb = list()


# 博弈网络  迭代更新节点状态，直至收敛
class Net:
    def __init__(self, n):
        self.numbers = n
        self.nodes = list()
        self.edges = list()
        self.reward = 0
        self.initNode()

    # 打印状态
    def printState(self):
        self.getAllReward()
        print('各个节点的决策和其收益：')
        for i in range(self.numbers):
            print('Noede%d:%c ==> reward: %f ' %
                  (i, self.nodes[i].state, self.nodes[i].value))
        print("博弈网络总体回报为:", self.reward)

    # 生成n个结点
    def initNode(self):
        for _ in range(self.numbers):
            tmp_node = Node()
            self.nodes.append(tmp_node)
            print(tmp_node.state)  # 打印初始状态

    # 根据传入的边的列表构建网络
    def buideNet(self, es):
        for e in es:
            self.edges.append(e)
        self.updateNb()

    # 更新每个节点的邻居
    def updateNb(self):
        for a, b in self.edges:
            a.nb.append(self.nodes.index(b))
            b.nb.append(self.nodes.index(a))
        for i in range(self.numbers):
            self.nodes[i].neighbour_number = len(self.nodes[i].nb)

    # 计算每个结点的平均收益,每条边的收益和/边数
    def calValue(self):
        for i in range(self.numbers):
            self.nodes[i].all_value = 0
        for a, b in self.edges:
            a.all_value += rewardMat[a.state][b.state][0]
            b.all_value += rewardMat[a.state][b.state][1]
        for i in range(self.numbers):
            self.nodes[i].value = self.nodes[i].all_value / self.nodes[i].neighbour_number

    # 每次改变一个参与人的策略，增加自己的收益
    # 根据是否有节点发生变化返回是否发生改变的标志
    def updateState(self):
        self.calValue()
        flag = False
        for i in range(self.numbers):
            if self.nodes[i].state == 'C':
                reward1 = self.getReward(i)
                self.nodes[i].state = 'D'
                reward2 = self.getReward(i)
                if reward2 <= reward1:
                    self.nodes[i].state = 'C'
                    continue
                flag = True
            elif self.nodes[i].state == 'D':
                reward1 = self.getReward(i)
                self.nodes[i].state = 'C'
                reward2 = self.getReward(i)
                if reward2 <= reward1:
                    self.nodes[i].state = 'D'
                    continue
                flag = True
        return flag

    # 计算单个结点的收益，某节点的所有边的收益/邻居个数
    def getReward(self, i):
        all_value = 0
        for s in self.nodes[i].nb:
            all_value += rewardMat[self.nodes[i].state][self.nodes[s].state][0]
        value = all_value / self.nodes[i].neighbour_number
        return value

    # 获得博弈网络的总收益
    def getAllReward(self):
        for a, b in self.edges:
            self.reward += sum(rewardMat[a.state][b.state])
        return self.reward


if __name__ == '__main__':
    net = Net(15)
    # 博弈网络中边的集合
    edge_list = [(0, 1), (0, 2), (0, 6), (0, 7),
                 (1, 2), (1, 4), (1, 9),
                 (2, 8), (2, 10), (2, 11),
                 (3, 5), (3, 12),
                 (4, 14),
                 (5, 13),
                 (6, 11), (6, 14),
                 (8, 9),
                 (12, 14)
                 ]

    net.buideNet(({net.nodes[a], net.nodes[b]} for a, b in edge_list))

    # 循环更新节点状态，直至每个人都不愿改变自己的决策
    while net.updateState():
        net.printState()  # 打印中间状态
    net.printState()
