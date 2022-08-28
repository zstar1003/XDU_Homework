import numpy as np
import matplotlib.pyplot as plt
import random


# 正常导入数据
def load_dataset():
    data = np.genfromtxt('./sonar.txt', delimiter=',', usecols=np.arange(0, 60))
    target = np.genfromtxt('./sonar.txt', delimiter=',', usecols=(60), dtype=str)
    t = np.zeros(len(target))
    t[target == 'R'] = 1
    t[target == 'M'] = 2
    return data, t


# 随机初始化k个聚类中心，从样本中随机选取
def randChosenCent(data, k):
    # 样本数
    m = data.shape[0]
    # 初始化列表
    centroids = []
    # 生成类似于样本索引的列表
    centroidsIndex = random.sample(range(0, m), k)  # 产生k个[0,60)的不同随机数
    # 根据索引获取样本
    for j in centroidsIndex:
        centroids.append(data[j])
    return centroids


def osdistance(vecA, vecB):  # 两个向量间欧式距离
    return np.sqrt(sum(np.power(vecA - vecB, 2)))


def kMeans(data, k):
    # 样本总数
    m = len(data)
    # 分配样本到最近的簇：存[簇序号,距离]，m行2列
    cluster = np.zeros((m, 2))

    # 通过随机产生的样本点初始化聚类中心
    centroids = np.array(randChosenCent(data, k))
    # print('最初的中心=', centroids)
    clusterChanged = True  # 标记每次迭代后聚类中心是否发生变化
    iterTime = 0  # 标记迭代次数
    # 所有样本分配结果不再改变，迭代终止
    while clusterChanged:
        # 分配到最近的聚类中心对应的簇中
        for i in range(m):
            # 初始定义距离为无穷大
            minDist = float('inf')
            # 初始化索引值
            minIndex = -1
            # 计算每个样本与k个中心点距离
            for j in range(k):
                # 计算第i个样本到第j个中心点的距离
                distJI = osdistance(centroids[j], data[i])
                # 判断距离是否为最小
                if distJI < minDist:
                    # 更新获取到最小距离
                    minDist = distJI
                    # 获取对应的簇序号
                    minIndex = j
            cluster[i, 0] = minIndex
            cluster[i, 1] = minDist
        iterTime += 1
        # 更新聚类中心
        centroids_pre = centroids.copy()  # 将之前的聚类中心做深拷贝
        for cent in range(k):
            cent_sum = np.zeros((1, 60))  # (1,60)维度的向量
            num = 0  # num 用来计量簇内个数
            for i in range(m):
                if (cluster[i, 0] == cent):
                    cent_sum += data[i, :]
                    num += 1
            centroids[cent, :] = cent_sum / num
        if ((centroids_pre == centroids).all()):
            clusterChanged = False
    # print('迭代次数为', '%d' % iterTime)
    return cluster, iterTime, centroids


# 计算分类准确率
def cal_accuracy(k):
    accuracy = 0
    for i in range(k):
        label_list = []  # label_list 存储第i簇样本的真实标签
        for j in range(len(cluster)):
            if (cluster[j][0] == i):
                label_list.append(t[j])
        # print(label_list)
        true_label = max(label_list, key=label_list.count)  # 选取数量最大的标签作为其标签
        # 再次遍历真实样本类别，若真实样本类别=簇类别，accuracy+1
        for n in range(len(label_list)):
            if (label_list[n] == true_label):
                accuracy += 1
    accuracy = accuracy / len(data)
    return accuracy


def draw(data, t):
    x1 = data[t == 1]
    x2 = data[t == 2]
    plt.figure(1)
    plt.scatter(x1[:, 0], x1[:, 1], c='g', marker='o', label='R')
    plt.scatter(x2[:, 0], x2[:, 1], c='blue', marker='o', label='M')
    plt.xlabel('特征一')
    plt.ylabel('特征二')
    plt.title('特征一和特征二之间的散点图（真实数据）')
    plt.legend(loc=2)  # 把图例放到左上角
    plt.rcParams['font.sans-serif'] = ['SimHei']
    plt.savefig('./sonar_kmeans(yuanshi)')
    plt.show()


def draw_pre(cluster, data, centroids):
    x1 = []
    x2 = []
    for i in range(len(cluster)):
        if cluster[i][0] == 0:
            x1.append(data[i])
        elif cluster[i][0] == 1:
            x2.append(data[i])
    x1 = np.array(x1)
    x2 = np.array(x2)
    plt.figure(2)
    plt.scatter(x1[:, 0], x1[:, 1], c='g', marker='o', label='R')
    plt.scatter(x2[:, 0], x2[:, 1], c='b', marker='o', label='M')
    plt.scatter(centroids[:, 0], centroids[:, 1], c='black', marker='x')
    plt.xlabel('特征一')
    plt.ylabel('特征二')
    plt.title('特征一和特征二之间的散点图（预测数据）')
    plt.legend(loc=2)  # 把图例放到左上角
    plt.rcParams['font.sans-serif'] = ['SimHei']
    plt.savefig('./sonar_kmeans(yuce)')
    plt.show()


if __name__ == '__main__':
    data, t = load_dataset()  # data(208,60)
    k = 2
    sum_iterTime = 0
    sum_accuracy = 0
    # 绘制前后对比散点图
    cluster, iterTime, centroids = kMeans(data, k)
    draw(data, t)
    draw_pre(cluster, data, centroids)
    for i in range(10):
        cluster, iterTime, centroids= kMeans(data, k)
        accuracy = cal_accuracy(k)
        sum_iterTime += iterTime
        sum_accuracy += accuracy
    print("平均迭代次数为:", "{}".format(sum_iterTime / 10))
    print("平均分类纯度为:", "{:.2%}".format(sum_accuracy / 10))
