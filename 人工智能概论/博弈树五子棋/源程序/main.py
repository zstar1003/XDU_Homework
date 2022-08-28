import pygame
import pygame.ftfont
from pygame import *
import sys
import button
import random
import re
import copy

colors = {
    "white": (255, 255, 255),
    "black": (0, 0, 0)
}
r_lines = [
    ((20, 20), (20, 620)),
    ((20, 620), (620, 620)),
    ((20, 20), (620, 20)),
    ((620, 20), (620, 620))
]
# 棋形匹配
l = [[] for i in range(5)]
l[4] = ["11111"]
l[3] = ["011110", "211110"]
l[2] = ["11101", "01110", "211100", "2011102"]
l[1] = ["011010", "11011", "211010", "210110", "11001", "001100", "211000"]
l[0] = ["10101", "01010", "010010", "210100", "210010", "10001"]
# 棋形与分值索引
values = {
    # 连五
    "11111": 999900000,
    # 活四
    "011110": 33300000,
    # 冲四
    "211110": 6250000,
    "11101": 6250000,
    "11011": 6250000,
    # 活三
    "01110": 625000,
    "011010": 625000,
    # 眠三
    "211100": 12500,
    "211010": 12500,
    "210110": 12500,
    "11001": 12500,
    "10101": 12500,
    "2011102": 12500,
    # 活二
    "001100": 250,
    "01010": 250,
    "010010": 250,
    # 眠二
    "211000": 25,
    "210100": 25,
    "210010": 25,
    "10001": 25,
    # 其他
    "others": 8
}
# 位置得分， 越靠近中心位置分越高
values_of_pos = [
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0],
    [0, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 0],
    [0, 2, 4, 6, 6, 6, 6, 6, 6, 6, 6, 6, 4, 2, 0],
    [0, 2, 4, 6, 7, 7, 7, 7, 7, 7, 7, 6, 4, 2, 0],
    [0, 2, 4, 6, 7, 8, 8, 8, 8, 8, 7, 6, 4, 2, 0],
    [0, 2, 4, 6, 7, 8, 9, 9, 9, 8, 7, 6, 4, 2, 0],
    [0, 2, 4, 6, 7, 8, 9, 10, 9, 8, 7, 6, 4, 2, 0],
    [0, 2, 4, 6, 7, 8, 9, 9, 9, 8, 7, 6, 4, 2, 0],
    [0, 2, 4, 6, 7, 8, 8, 8, 8, 8, 7, 6, 4, 2, 0],
    [0, 2, 4, 6, 7, 7, 7, 7, 7, 7, 7, 6, 4, 2, 0],
    [0, 2, 4, 6, 6, 6, 6, 6, 6, 6, 6, 6, 4, 2, 0],
    [0, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 0],
    [0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
]
me_first = 1
ai_first = 0  # 设置先后手规则
first = me_first  # 默认先手为玩家， 即玩家持黑
Myturn = 1  # 玩家回合
AIturn = 0  # 电脑回合
flag = Myturn  # 设置轮流黑白手标志
game_state = 0  # 游戏状态, 0为未决胜负, 1为黑子胜, -1为白子胜
b_num = 0  # 黑子个数
w_num = 0  # 白子个数
MAX = 100000000000  # 正无穷
MIN = -100000000000  # 负无穷
deep = 3  # 规定ai搜索深度
drawed = True  # 画出一方的棋另一方再走
point = [[-1] * 3 for i in range(200)]  # 存储n中走法对应的点的横坐标[n][0]纵坐标[n][1]及value值[n][2]
num_points = 0  # 记录可走步的个数
p1 = [0, 0]  #
p2 = [0, 0]  #

pygame.init()  # 初始化pygame
pygame.mixer.init()  # 初始化mixer
# 设置屏幕大小和标题
size = width, height = 740, 640  # 设置窗口大小
screen = pygame.display.set_mode(size)  # 显示窗口
pygame.display.set_caption("博弈树五子棋")
font = pygame.font.SysFont('comicsansms', 16)  # 字体
ai_text = font.render('AI Steps', True, (84, 115, 135))
player_text = font.render('Player Steps', True, (84, 115, 135))

clock = pygame.time.Clock()  # 设置时钟，定时刷新
whiteq = []  # 存储白棋位置
blackq = []  # 存储黑棋位置
chess = [[0] * 15 for i in range(15)]  # 用来判断棋盘某处是否有棋,1为黑棋, -1为白棋, 0为空


# 更新chess状态
def renew_chess(chess, blackq, whiteq):
    for xx in range(15):
        for yy in range(15):
            chess[xx][yy] = 0
    for xx, yy in blackq:
        chess[int((yy - 20) / 40)][int((xx - 20) / 40)] = 1  # 黑棋
    for xx, yy in whiteq:
        chess[int((yy - 20) / 40)][int((xx - 20) / 40)] = -1  # 白棋


# 按钮响应函数
def do_start(btn):  # 开始按钮的响应函数
    global game_state, flag, b_num, w_num, blackq, whiteq
    game_state = 0  # 重置游戏状态及其他全局变量
    b_num = 0
    w_num = 0
    blackq = []
    whiteq = []
    if first == me_first:
        flag = Myturn
    if first == ai_first:
        flag = AIturn


def do_quit(btn):  # 退出按钮的响应函数
    pygame.quit()
    exit()


def do_ai_first(btn):  # 电脑先手
    global first, flag
    first = ai_first
    do_start(btn)


def do_me_first(btn):  # 玩家先手
    global first, flag
    first = me_first
    do_start(btn)


# 画棋盘背景
def draw_background(screen):
    # 加载背景图片
    bg_image = pygame.image.load('bg.png')  # 加载图片
    screen.blit(bg_image, (0, 0))
    # 画网格线，15x15
    for line in r_lines:
        pygame.draw.line(screen, colors["black"], line[0], line[1], 2)
    for i in range(15):
        pygame.draw.line(screen, colors["black"], (20, 20 + 40 * i), (620, 20 + 40 * i), 1)  # 画行
        pygame.draw.line(screen, colors["black"], (20 + 40 * i, 20), (20 + 40 * i, 620), 1)  # 画列


# 画棋子
def draw_gos(screen, blackq):
    # 画黑棋
    for xq, yq in blackq:
        b_ = pygame.image.load('black.png')  # 加载图片
        b_rect = b_.get_rect()
        b_rect.centerx = xq
        b_rect.centery = yq
        screen.blit(b_, b_rect)
    # 画白棋
    for xq, yq in whiteq:
        w_ = pygame.image.load('white.png')  # 加载图片
        w_rect = w_.get_rect()
        w_rect.centerx = xq
        w_rect.centery = yq
        screen.blit(w_, w_rect)


# 获取以p点为中心的9个点的连线棋形
def getline(chess, px, py, dir_offset, discolor):
    line = [0 for i in range(9)]
    tmp_x = px + (-5 * dir_offset[0])
    tmp_y = py + (-5 * dir_offset[1])
    for i in range(9):
        tmp_x += dir_offset[0]
        tmp_y += dir_offset[1]
        if (tmp_x < 0 or tmp_x >= 15 or
                tmp_y < 0 or tmp_y >= 15):
            line[i] = discolor  # 若超界则假设为对手棋
        else:
            line[i] = chess[tmp_y][tmp_x]  # 黑1白-1
    return line


# 分析棋形并得到该方向的分值,最后的参数是,有利分为1不利分为-1
def get_line_score(line, adcolor, discolor, weight, px, py):
    global l, values
    new_line = [[] for i in range(9)]
    num = 0  # 统计连子个数
    for i in range(line.index(adcolor), 9):
        if line[i] == adcolor:
            num += 1
    # 将line格式统一化,转字符串,己方棋子为1，对方棋子为2，空为0，便于匹配
    for i in range(9):
        if line[i] == adcolor:
            new_line[i] = '1'
        elif line[i] == discolor:
            new_line[i] = '2'
        else:
            new_line[i] = '0'
    s = ''.join(new_line)  # 转化为字符串
    fan_s = s[::-1]  # 字符串反转
    # 逐个匹配
    if num > 5:
        return values["11111"] * weight
    if 5 >= num >= 1:
        for each in l[num - 1]:
            result = re.search(each, s)
            if result != None:  # 找到匹配项
                return (values[result.group()] + values_of_pos[py][px]) * weight
            result = re.search(each, fan_s)
            if result != None:  # 找到匹配项
                return (values[result.group()] + values_of_pos[py][px]) * weight
    # 未找到匹配项，为单子情况
    return (values["others"] + values_of_pos[py][px]) * weight


# 评估函数，评估当前棋局的分值
def Evaluate(chess):
    advantage = 0  # 有利分, 对上一层有利的分！！
    disadvantage = 0  # 不利分, 玩家所持棋子得分, 取负
    adcolor = 0  # ai所持棋子颜色
    discolor = 0  # 玩家所持棋子颜色
    dir_offset = [(1, 0), (0, 1), (1, 1), (1, -1)]  # 四个方向，右，上，右上，右下
    if first == me_first:
        adcolor = 1  # ai持白子
        discolor = -1
    if first == ai_first:
        adcolor = -1  # ai持黑子
        discolor = 1
    for k in range(4):
        line = getline(chess, p2[0], p2[1], dir_offset[k], discolor)  # 获得此方向的连线棋形
        advantage += get_line_score(line, adcolor, discolor, 1, p2[0], p2[1])  # 分析棋形并得到该方向的分值  有利得分, 权重小于1以增强攻击性
        line = getline(chess, p1[0], p1[1], dir_offset[k], adcolor)  # 获得此方向的连线棋形
        disadvantage += get_line_score(line, discolor, adcolor, -1, p1[0], p1[1])  # 分析棋形并得到该方向的分值  不利得分
    return advantage + disadvantage

# 获取下一步所有可能走法的点位
def get_next_pos(tchess, b_num):
    num_pos = 0
    positions = [[-1] * 2 for i in range(200)]
    top = 0
    borrow = 0
    left = 0
    right = 0
    find = False
    for i in range(15):
        if find:
            break
        for j in range(15):
            if tchess[i][j] != 0:  # 该处有棋
                top = i  # 此棋局棋子落下的范围-上
                find = True
                break
    find = False
    for i in range(15):
        if find:
            break
        for j in range(15):
            if tchess[14 - i][j] != 0:  # 该处有棋
                borrow = 14 - i  # 此棋局棋子落下的范围-下
                find = True
                break
    find = False
    for i in range(15):
        if find:
            break
        for j in range(15):
            if tchess[j][i] != 0:  # 该处有棋
                left = i  # 此棋局棋子落下的范围-左
                find = True
                break
    find = False
    for i in range(15):
        if find:
            break
        for j in range(15):
            if tchess[j][14 - i] != 0:  # 该处有棋
                right = 14 - i  # 此棋局棋子落下的范围-右
                find = True
                break
    if b_num <= 10:
        scope = 1
    else:
        scope = 1
    if top - scope >= 0:  # 假设下一步走的棋的位置不超过当前棋局范围四格， 缩小范围，提高效率
        top -= scope
    else:
        top = 0
    if borrow + scope < 15:
        borrow += scope
    else:
        borrow = 14
    if left - scope >= 0:
        left -= scope
    else:
        left = 0
    if right + scope < 15:
        right += scope
    else:
        right = 14
    for i in range(top, borrow + 1):
        for j in range(left, right + 1):
            if tchess[i][j] == 0:  # 该处可以走步
                positions[num_pos][0] = j
                positions[num_pos][1] = i  # 记下可走步的x,y
                num_pos += 1
    return positions, num_pos


# 将p点放入棋盘布局
def set_newchess(new_chess, p, max_or_min):
    # max(1), min(-1), 黑棋(1), 白棋(-1), me_first(1), ai_first(0)
    if first == me_first:
        new_chess[p[1]][p[0]] = max_or_min
    if first == ai_first:
        new_chess[p[1]][p[0]] = -1 * max_or_min


# alpha-beta剪枝搜索，返回最好的值
def alpha_beta(depth, max_or_min, chess, alpha, beta):
    global point, num_points
    if depth == 1:  # 最后一层
        return Evaluate(chess)  # 评估函数，评估当前棋局的分值,pp为此棋局的最后一个子的位置
    positions, num_pos = get_next_pos(chess, b_num)  # 获取下一步所有可能走法的点位
    if depth == deep:
        num_points = num_pos
        for i in range(num_points):
            point[i][0], point[i][1] = positions[i][0], positions[i][1]  # 保存可能走到的位置的x,y
    if max_or_min == 1:  # MAX层
        # 对于每一个可能的走步
        for p in range(num_pos):
            new_chess = copy.deepcopy(chess)  # 拷贝旧棋局
            set_newchess(new_chess, positions[p], max_or_min)  # 将此步放置到新棋局上
            p2[0], p2[1] = positions[p][0], positions[p][1]
            score = alpha_beta(depth - 1, -1 * max_or_min, new_chess, alpha, beta)  # 得到它的下一层的值score
            if depth == deep:
                point[p][2] = score
            if score > alpha:
                alpha = score
            if alpha >= beta:
                return alpha  # 此处为aplha-beta剪枝
        return alpha
    if max_or_min == -1:  # MIN层
        for p in range(num_pos):  # 对于每一个可能的走步
            new_chess = copy.deepcopy(chess)  # 拷贝旧棋局
            set_newchess(new_chess, positions[p], max_or_min)  # 将此步放置到新棋局上
            p1[0], p1[1] = positions[p][0],positions[p][1]
            win = check_win_in_alpha_beta(new_chess)  # 检查此步是否为绝杀，若为绝杀直接返回
            if win == MIN:
                beta = MIN
                point[i][2] = MIN
                return beta
            score = alpha_beta(depth - 1, -1 * max_or_min, new_chess, alpha, beta)  # 得到它的下一层的值score
            if depth == deep:
                point[p][2] = score
            if score < beta:
                beta = score
            if beta <= alpha:
                return beta  # 此处为aplha-beta剪枝
        return beta


# 获取对应value值的走法的位置
def get_point(value):
    global point, num_points
    for i in range(num_points):
        if point[i][2] == value:
            return int(point[i][0] * 40 + 20), int(point[i][1] * 40 + 20)

# AI下棋
def draw_ai(blackq, whiteq):
    global b_num, w_num, flag, game_state, point, num_points, p1, p2
    p1 = [0, 0]
    p2 = [0, 0]
    # 若ai先手， ai随机走第一步
    not_first = True  # 不是第一步的标志
    if first == ai_first and b_num == 0:
        xq = random.randint(6, 8)  #
        yq = random.randint(6, 8)  #
        xq = 20 + xq * 40
        yq = 20 + yq * 40
        while (xq, yq) in whiteq or (xq, yq) in blackq:
            xq = random.randint(0, 14)  #
            yq = random.randint(0, 14)  #
            xq = 20 + xq * 40
            yq = 20 + yq * 40
        if first == me_first:
            whiteq.append((xq, yq))  # 把位置添加进数组
            w_num += 1
        if first == ai_first:
            blackq.append((xq, yq))
            b_num += 1
        flag = Myturn  # 下一轮玩家
        not_first = False
        return
    if first == me_first or (first == ai_first and not_first):  # ai为MIN层
        max_or_min = -1
        point = [[-1] * 3 for i in range(200)]  # 存储n中走法对应的点的横坐标[n][0]纵坐标[n][1]及value值[n][2]
        num_points = 0  # 记录可走步的个数
        # 利用极大极小值搜索和alpha-beta剪枝获取最小value值
        value = alpha_beta(deep, max_or_min, chess, MIN, MAX)  # alpha初始为负无穷，beta初始为正无穷
        # 得到value对应的走法位置（若有多个则随机选择一个）
        xq, yq = get_point(value)  # 获取value值对应的走法位置
        # 将找到的点的坐标拓展
        if first == me_first:
            whiteq.append((xq, yq))  # 把位置添加进数组
            w_num += 1
        if first == ai_first:
            blackq.append((xq, yq))
            b_num += 1
    check_win()  # 检查是否出胜负
    if game_state == 0:
        flag = Myturn  # 下一轮玩家


# 获取棋子在某个方向上连续的最大个数,最后的参数是棋子颜色对应的标记
def one_dir_num(x, y, dx, dy, chess, b_or_w):
    xi = int((x - 20) / 40)
    yi = int((y - 20) / 40)
    sum = 0
    while True:
        xi += dx
        yi += dy
        if xi < 0 or xi >= 15 or yi < 0 or yi >= 15 or chess[yi][xi] != b_or_w:
            return sum
        sum += 1


def check_win_in_alpha_beta(chess):
    # 四个方向数组
    director = [[(-1, 0), (1, 0)], [(0, -1), (0, 1)], [(-1, -1), (1, 1)], [(-1, 1), (1, -1)]]
    if first == me_first:
        color = -1
    if first == ai_first:
        color = 1
    # 检查ai所持棋子
    # 遍历每一个该色棋子
    for i in range(15):
        for j in range(15):
            if chess[i][j] == color:
                for d1, d2 in director:
                    dx, dy = d1
                    num1 = one_dir_num(int(j*40+20), int(i*40+20), dx, dy, chess, color)
                    dx, dy = d2
                    num2 = one_dir_num(int(j*40+20), int(i*40+20), dx, dy, chess, color)
                    if num1 + num2 >= 5:
                        return  MIN  # 赢
    return -1


# 判断胜负
def check_win():
    global chess, blackq, whiteq, game_state
    renew_chess(chess, blackq, whiteq)  # 更新状态数组
    # 四个方向数组
    director = [[(-1, 0), (1, 0)], [(0, -1), (0, 1)], [(-1, -1), (1, 1)], [(-1, 1), (1, -1)]]
    b_last = b_num - 1 if b_num != 0 else 0
    w_last = w_num - 1 if w_num != 0 else 0

    # 检查黑子
    if b_last != 0:
        b_lx = blackq[b_last][0]  # 黑子最后一个子x
        b_ly = blackq[b_last][1]  # y
        for d1, d2 in director:
            dx, dy = d1
            b_num1 = one_dir_num(b_lx, b_ly, dx, dy, chess, 1)
            dx, dy = d2
            b_num2 = one_dir_num(b_lx, b_ly, dx, dy, chess, 1)
            if b_num1 + b_num2 + 1 >= 5:
                game_state = 1
                return  # 黑子赢
    # 检查白子
    if w_last != 0:
        w_lx = whiteq[w_last][0]  # 白子最后一个子x
        w_ly = whiteq[w_last][1]  # y
        for d1, d2 in director:
            dx, dy = d1
            w_num1 = one_dir_num(w_lx, w_ly, dx, dy, chess, -1)
            dx, dy = d2
            w_num2 = one_dir_num(w_lx, w_ly, dx, dy, chess, -1)
            if w_num1 + w_num2 + 1 >= 5:
                game_state = -1
                return  # 白子赢
    return  # 未决胜负


# 显示甲乙方信息
def show(screen):
    # 显示甲方乙方已走步数
    global time_AI, time_Player
    tnr = pygame.font.SysFont('Times New Roman', 20)
    if first == me_first:  # 玩家先手时
        time_AI = str(w_num)
        time_Player = str(b_num)
    if first == ai_first:  # 电脑先手时
        time_AI = str(b_num)
        time_Player = str(w_num)
    text1 = tnr.render(time_AI, False, (84, 115, 135))
    text2 = tnr.render(time_Player, False, (84, 115, 135))
    screen.blit(text1, (640, 70))
    screen.blit(text2, (640, 150))
    # 显示甲乙方所持棋子
    b_ = pygame.image.load('black.png')  # 加载图片
    b_rect = b_.get_rect()
    b_rect.centerx = 680
    b_rect.centery = 170 if first == me_first else 90
    screen.blit(b_, b_rect)
    w_ = pygame.image.load('white.png')  # 加载图片
    w_rect = w_.get_rect()
    w_rect.centerx = 680
    w_rect.centery = 90 if first == me_first else 170
    screen.blit(w_, w_rect)


# 创建开始按钮
button_go = button.BFButton(screen, (635, 480, 90, 40), text='Replay', click=do_start)
# 退出按钮
button_quit = button.BFButton(screen, (635, 560, 90, 40), text='Quit', click=do_quit)
# 电脑先手按钮
button_ai_first = button.BFButton(screen, (635, 280, 90, 40), text='AI First', click=do_ai_first)
# 玩家先手按钮
button_me_first = button.BFButton(screen, (635, 360, 90, 40), text='Me First', click=do_me_first)

while True:  # 死循环确保窗口一直显示
    clock.tick(60)  # 屏幕刷新频率，每秒执行60次
    if first == me_first:
        mouse_cursor = pygame.image.load('black.png')  # 鼠标图片
    else:
        mouse_cursor = pygame.image.load('white.png')  # 鼠标图片
    x, y = pygame.mouse.get_pos()  # 鼠标的坐标
    pygame.mouse.set_visible(False)  # 隐藏鼠标
    x -= mouse_cursor.get_width()/2
    y -= mouse_cursor.get_height()/2
    #screen.blit(mouse_cursor, (x, y))  # 用棋子代替鼠标
    for event in pygame.event.get():  # 遍历所有事件
        button_go.update(event)
        button_quit.update(event)
        button_me_first.update(event)
        button_ai_first.update(event)  # 按钮置于激活状态
        if event.type == pygame.QUIT:  # 如果单击关闭窗口，则退出
            sys.exit()
        # AI下棋
        if game_state == 0 and flag == AIturn and drawed:
            draw_ai(blackq, whiteq)
            drawed = False
        # 玩家下棋
        if event.type == pygame.MOUSEBUTTONUP and game_state == 0 and flag == Myturn and drawed:  # 鼠标弹起事件,且为游戏中状态和我方回合
            x, y = pygame.mouse.get_pos()  # 获取鼠标所在位置
            if 20 <= x < 620 and 20 <= y < 620:
                xi = int(round((x - 20) * 1.0 / 40))  # 位置取整
                yi = int(round((y - 20) * 1.0 / 40))
                x = xi * 40 + 20
                y = yi * 40 + 20
                if 0 <= xi < 15 and 0 <= yi < 15 \
                        and (x, y) not in blackq and (x, y) not in whiteq:
                    if first == me_first:  # 玩家执黑时
                        blackq.append((x, y))  # 把位置添加进数组
                        b_num += 1
                    if first == ai_first:  # 玩家执白时
                        whiteq.append((x, y))  # 把位置添加进数组
                        w_num += 1
                    check_win()  # 检查是否出胜负
                    if game_state == 0:
                        flag = AIturn  # 下一轮AI
                        drawed = False

    screen.fill((219, 207, 202))  # 填充颜色
    screen.blit(ai_text, (640, 40))  # 文字
    screen.blit(player_text, (640, 120))
    # 画棋盘
    draw_background(screen)
    # 画棋子
    draw_gos(screen, blackq)
    button_go.draw()  # 生成开始按钮
    button_quit.draw()  # 生成退出按钮
    button_ai_first.draw()  # 电脑先手按钮
    button_me_first.draw()  # 玩家先手按钮

    drawed = True
    # 显示甲乙方信息
    show(screen)
    # 显示胜负信息
    if game_state != 0:
        myfont = pygame.font.SysFont('comicsansms', 100)
        me_first_win_text = "You %s" % ('Win!!!' if game_state == 1 else 'Lose...')
        ai_first_win_text = "You %s" % ('Win!!!' if game_state == -1 else 'Lose...')
        if first == me_first:
            textImage = myfont.render(me_first_win_text, True, (84, 115, 135))
        if first == ai_first:
            textImage = myfont.render(ai_first_win_text, True, (84, 115, 135))
        screen.blit(textImage, (100, 100))
    screen.blit(mouse_cursor, (x, y))  # 用棋子代替鼠标
    # 刷新屏幕，更新全部显示
    pygame.display.flip()

pygame.quit()  # 退出pygame
