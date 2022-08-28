import numpy as np

# 正常导入数据
def load_dataset():
    data = np.genfromtxt('./sonar.txt', delimiter=',', usecols=np.arange(0, 60))
    target = np.genfromtxt('./sonar.txt', delimiter=',', usecols=(60), dtype=str)
    t = np.zeros(len(target))
    t[target == 'R'] = 1
    t[target == 'M'] = 2
    return data, t


def osdistance(vecA, vecB):  # 两个向量间欧式距离
    return np.sqrt(sum(np.power(vecA - vecB, 2)))



# 初始化U矩阵
def initmatU(m, c):
    mat_u = np.random.uniform(0, 1, (m, c))  # 0,1之间均匀分布初始化
    # 归一化——每一个样本对所有分类集合隶属度总和为1
    for i in range(m):
        addsum = 0
        for j in range(c):
            addsum += mat_u[i, j]
        mat_u[i, :] = mat_u[i, :] / addsum
    return mat_u


def FCMtrain(data, c, alpha, theta):
    m = len(data)
    dim = data.shape[1]  # 样本维度
    mat_u = initmatU(m, c)
    # 计算c个聚类中心
    c_list = np.zeros([c, dim])
    iterTime = 0  # 标记迭代次数
    last_cost = 0  # 上一次的损失

    while True:
        # 计算聚类中心c_list
        for j in range(c):
            sum_uij = 0  # 表达式分母
            sum_uij_x = 0  # 表达式分子
            for i in range(m):
                sum_uij += mat_u[i, j] ** alpha
                sum_uij_x += mat_u[i, j] ** alpha * data[i, :]
            c_list[j, :] = sum_uij_x / sum_uij
        # 计算损失函数
        cost = 0
        for j in range(c):
            for i in range(m):
                vec1 = np.array(data[i, :])  # 第i条样本
                vec2 = np.array(c_list[j, :])  # 第j个中心
                dis = osdistance(vec1, vec2)
                cost += mat_u[i, j] ** alpha * dis ** 2
        if abs(last_cost - cost) < theta:
            break
        last_cost = cost
        # 重新计算U
        for j in range(c):
            vec1 = np.array(c_list[j, :])  # 第j条样本
            for i in range(m):
                vec2 = np.array(data[i, :])  # 第i个中心
                dis_ij = osdistance(vec1, vec2)
                sumd_d = 0
                for k in range(c):
                    vec3 = np.array(c_list[k, :])  # 第k个中心
                    dis_ki = osdistance(vec2, vec3)
                    sumd_d += (dis_ij / dis_ki) ** (2 / (alpha - 1))
                mat_u[i, j] = 1 / sumd_d
        # 归一化
        for i in range(m):
            addsum = 0
            for j in range(c):
                addsum += mat_u[i, j]
            mat_u[i, :] = mat_u[i, :] / addsum
        iterTime += 1
    # print('迭代次数为', '%d' % iterTime)
    # 对每一条样本进行遍历，隶属度最大的集合类别即为样本预测类别
    pred = []
    for i in range(m):
        t = np.argmax(mat_u[i, :])
        pred.append(t)
    return c_list, pred, iterTime


# 计算分类准确率
def cal_accuracy(c, pred):
    accuracy = 0
    for i in range(c):
        label_list = []  # label_list 存储第i簇样本的真实标签
        for j in range(len(pred)):
            if (pred[j] == i):
                label_list.append(t[j])
        true_label = max(label_list, key=label_list.count)  # 选取数量最大的标签作为其标签
        # 再次遍历真实样本类别，若真实样本类别=簇类别，accuracy+1
        for n in range(len(label_list)):
            if (label_list[n] == true_label):
                accuracy += 1
    accuracy = accuracy / len(data)
    return accuracy


if __name__ == '__main__':
    data, t = load_dataset()  # data(208,60)
    c = 2
    alpha = 3
    theta = 0.001
    sum_iterTime = 0
    sum_accuracy = 0
    for i in range(10):
        c_list, pred, iterTime = FCMtrain(data, c, alpha, theta)
        accuracy = cal_accuracy(c, pred)
        sum_iterTime += iterTime
        sum_accuracy += accuracy
    print("平均迭代次数为:", "{}".format(sum_iterTime/10))
    print("平均分类纯度为:", "{:.2%}".format(sum_accuracy/10))
