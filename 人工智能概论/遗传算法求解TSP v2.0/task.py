import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

plt.rcParams['font.sans-serif'] = ["SimHei"]


# 载入数据
def load_data():
    df = pd.read_csv('./TSP问题测试数据集/oliver30.tsp', sep=" ", skiprows=6, header=None)
    city = np.array(df[0][0:len(df) - 1])  # 最后一行为EOF，不读入
    city_name = city.tolist()
    city_x = np.array(df[1][0:len(df) - 1])
    city_y = np.array(df[2][0:len(df) - 1])
    city_location = list(zip(city_x, city_y))
    return city_name, city_location


# 计算两个城市的欧式距离
def dist_cal(x, y):
    return ((x[0] - y[0]) ** 2 + (x[1] - y[1]) ** 2) ** 0.5


# 求距离矩阵
def matrix_dis(city_name, city_location):
    city_num = len(city_name) - 1
    res = np.zeros((city_num, city_num))
    for i in range(city_num):
        for j in range(i + 1, city_num):
            res[i, j] = dist_cal(city_location[i], city_location[j])  # 求两点欧式距离
            res[j, i] = res[i, j]  # 距离矩阵：对角线为0，对称
    return res


# 初始化种群
def rand_chrom(city_num, pop_num, chrom, distance, matrix_distance):
    rand_ch = np.array(range(city_num))
    for i in range(pop_num):
        np.random.shuffle(rand_ch)
        chrom[i, :] = rand_ch
        distance[i] = comp_dis(city_num, matrix_distance, rand_ch)  # 这里的适应度其实是距离


# 计算每个染色体的总距离
def comp_dis(city_num, matrix_distance, one_path):
    res = 0
    for i in range(city_num - 1):
        res += matrix_distance[one_path[i], one_path[i + 1]]
    res += matrix_distance[one_path[-1], one_path[0]]  # 最后一个城市和第一个城市的距离，需单独处理
    return res


# 打印出当前路径
def print_path(city_num, one_path):
    res = str(one_path[0] + 1) + '-->'
    for i in range(1, city_num):
        res += str(one_path[i] + 1) + '-->'
    res += str(one_path[0] + 1)
    print("最佳路径为：")
    print(res)


# 轮盘赌的方式选择子代
def select_sub(pop_num, chrom, distance):
    fit = 1. / distance  # 适应度函数
    p = fit / sum(fit)
    q = p.cumsum()  # 累积概率
    select_id = []
    for i in range(pop_num):
        r = np.random.rand()  # 产生一个[0,1)的随机数
        for j in range(pop_num):
            if r < q[0]:
                select_id.append(0)
                break
            elif q[j] < r <= q[j + 1]:
                select_id.append(j + 1)
                break
    next_gen = chrom[select_id, :]
    return next_gen


# 交叉操作-每个染色体对的某一位置进行交叉
def cross_sub(city_num, pop_num, next_gen, cross_prob):
    # 若染色体对数量为偶数，进行数量/2次判断交叉；若奇数，最后一个染色体不交叉
    if pop_num % 2 == 0:
        cross_num = pop_num
    else:
        cross_num = pop_num - 1
    for i in range(0, cross_num, 2):
        if cross_prob >= np.random.rand():
            next_gen[i, :], next_gen[i + 1, :] = intercross(city_num, next_gen[i, :], next_gen[i + 1, :])


# 具体的交叉方式：部分映射交叉(Partial-Mapped Crossover)
def intercross(city_num, ind_a, ind_b):
    r1 = np.random.randint(city_num)
    r2 = np.random.randint(city_num)
    while r2 == r1:
        r2 = np.random.randint(city_num)
    left, right = min(r1, r2), max(r1, r2)
    ind_a1 = ind_a.copy()
    ind_b1 = ind_b.copy()
    for i in range(left, right + 1):
        ind_a2 = ind_a.copy()
        ind_b2 = ind_b.copy()
        ind_a[i] = ind_b1[i]
        ind_b[i] = ind_a1[i]
        # 每个染色体包含的城市序号是唯一的，因此交叉时若两个不相同，就会产生冲突
        x = np.argwhere(ind_a == ind_a[i])
        y = np.argwhere(ind_b == ind_b[i])
        # 产生冲突，将不是交叉区间的数据换成换出去的原数值，保证城市序号唯一
        if len(x) == 2:
            ind_a[x[x != i]] = ind_a2[i]
        if len(y) == 2:
            ind_b[y[y != i]] = ind_b2[i]
    return ind_a, ind_b


# 变异方式：双点变异—将染色体的某两个数值进行交换
def mutation_sub(city_num, pop_num, next_gen, mut_prob):
    for i in range(pop_num):
        if mut_prob >= np.random.rand():
            r1 = np.random.randint(city_num)
            r2 = np.random.randint(city_num)
            while r2 == r1:
                r2 = np.random.randint(city_num)
            next_gen[i, [r1, r2]] = next_gen[i, [r2, r1]]


# 绘制路径图
def draw_path(city_num, city_location, chrom, distance):
    fig, ax = plt.subplots()
    x, y = zip(*city_location)
    ax.scatter(x, y, linewidths=0.1)
    for i, txt in enumerate(range(1, len(city_location) + 1)):
        ax.annotate(txt, (x[i], y[i]))
    res0 = chrom
    x0 = [x[i] for i in res0]
    y0 = [y[i] for i in res0]
    ax.annotate("起点", (x0[0], y[0]))
    ax.annotate("终点", (x0[-1], y[-1]))
    # 绘制箭图
    for i in range(city_num - 1):
        plt.quiver(x0[i], y0[i], x0[i + 1] - x0[i], y0[i + 1] - y0[i], color='b', width=0.005, angles='xy', scale=1,
                   scale_units='xy')
    plt.quiver(x0[-1], y0[-1], x0[0] - x0[-1], y0[0] - y0[-1], color='b', width=0.005, angles='xy', scale=1,
               scale_units='xy')
    plt.title("遗传算法优化路径-最短距离:" + str(distance))
    plt.xlabel("城市位置横坐标")
    plt.xlabel("城市位置纵坐标")
    plt.savefig("att48_1.png")
    plt.show()


# 绘制最优解随迭代次数的关系
def draw_iter(iteration, best_distance_list):
    iteration = np.linspace(1, iteration, iteration)
    plt.plot(iteration, best_distance_list)
    plt.xlabel("迭代次数")
    plt.ylabel("最短路径长度")
    plt.savefig("time.png")
    plt.show()


def main():
    city_name, city_location = load_data()
    matrix_distance = matrix_dis(city_name, city_location)
    city_num = len(city_name) - 1  # 城市数量
    pop_num = 100  # 群体个数
    cross_prob = 0.95  # 交叉概率
    mut_prob = 0.1  # 变异概率
    iteration = 500  # 迭代代数

    # 初始化初代染色体和距离，染色体为整数，距离为浮点数
    chrom = np.array([0] * pop_num * city_num).reshape(pop_num, city_num)
    distance = np.zeros(pop_num)
    # 初始化种群
    rand_chrom(city_num, pop_num, chrom, distance, matrix_distance)
    # draw_path(city_num, city_location, chrom[0], distance)  # 绘制初代图像

    evbest_path = chrom[0]
    evbest_distance = float("inf")
    best_path_list = []
    best_distance_list = []
    # 循环迭代遗传过程
    for i in range(iteration):
        # 选择
        next_gen = select_sub(pop_num, chrom, distance)
        # 交叉
        cross_sub(city_num, pop_num, next_gen, cross_prob)
        # 变异
        mutation_sub(city_num, pop_num, next_gen, mut_prob)

        # 更新每个染色体距离值(1/适应度)
        for j in range(pop_num):
            distance[j] = comp_dis(city_num, matrix_distance, next_gen[j, :])
        index = distance.argmin()  # index 记录最小总路程

        # 为了防止曲线波动，每次记录最优值，如迭代后出现退化，则将当前最好的染色体回退替换为历史最佳
        if distance[index] <= evbest_distance:
            evbest_distance = distance[index]
            evbest_path = next_gen[index, :]
        else:
            distance[index] = evbest_distance
            next_gen[index, :] = evbest_path
        # 存储每一步的最优路径(染色体)及距离
        best_path_list.append(evbest_path)
        best_distance_list.append(evbest_distance)

    # 绘制迭代次数和最优解的关系曲线
    draw_iter(iteration, best_distance_list)

    # best_path = best_path_list[int(np.argmin(best_distance_list))]
    # best_distance = min(best_distance_list)
    best_path = evbest_path
    best_distance = evbest_distance
    # 迭代完成，打印出最佳路径
    print_path(city_num, best_path)
    print("当前最佳距离为:", best_distance)
    # 绘制路径图
    draw_path(city_num, city_location, best_path, best_distance)


if __name__ == '__main__':
    main()
