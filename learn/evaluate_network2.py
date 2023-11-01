# ====================
# 新パラメータ評価部
# ====================

# パッケージのインポート
from game import State
from pathlib import Path
from shutil import copy
import numpy as np
from single_network import *
import json

# ベストプレイヤーの交代
def update_best_player():
    copy('./model/latest_single.h5', './model/best_single.h5')
    print('Change BestPlayer')

def predict(model, state, device):
    feature = np.array(state.feature())
    file, rank, channel = DN_INPUT_SHAPE
    feature = feature.reshape(1, channel, file, rank)
    f = torch.from_numpy(feature)
    f = f.to(device).float()
    return model(f)

def read_file_to_list(file_path):
    data_list = []
    with open(file_path, 'r', encoding='utf-8') as file:
        for line in file:
            data_list.append(line.strip())  # 改行文字を削除してリストに追加
    return data_list

def evaluate_problem2():
    # ベストプレイヤーのモデルの読み込み
    device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
    model = SingleNet(drop=True)
    model.load_state_dict(torch.load('./model/best_single.h5'))
    model = model.to(device)
    model.eval()
    problem_list = read_file_to_list('../oracle/test.txt')   
    with open("output.csv", "a") as file:
        file.write("result,score,score2,mean,mean2,median,max,min,std\n")

    for i,problem in enumerate(problem_list):
        print(f"{i}/{len(problem_list)}")
        split_text_list = problem.split(":")
        key = split_text_list[0]
        result = int(split_text_list[1])
        state = from_hash2(key)
        score_list = []
        for j in range(1000):
            score = predict(model,state,device)
            score = score.item()
            score_list.append(score)

        lines = []

        score_list = np.array(score_list)
        model.drop = False
        score = predict(model,state,device).item()
        model.drop = True  
        score2 = 0
        mean2 = 0
        if score > 0.2:
            score2 = 1
        elif score < -0.2:
            score2 = -1
        if np.mean(score_list) > 0.2:
            mean2 = 1
        elif np.mean(score_list) < -0.2:
            mean2 = -1
        with open("output.csv", "a") as file:
            file.write(f"{result},{score},{score2},{np.mean(score_list)},{mean2},{np.median(score_list)},{np.max(score_list)},{np.min(score_list)},{np.std(score_list)}\n")

def evaluate_problem():
    # ベストプレイヤーのモデルの読み込み
    device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
    model = SingleNet(drop=True)
    model.load_state_dict(torch.load('./model/best_single.h5'))
    model = model.to(device)
    model.eval()

    problem_list = read_file_to_list('../oracle/test.txt')    
    
    correct_num = 0
    for i,problem in enumerate(problem_list):
        split_text_list = problem.split(":")
        key = split_text_list[0]
        result = int(split_text_list[1])
        state = from_hash2(key)
        score = predict(model,state,device)
        score = score.item()
        org_score = score
        if score > 0.2:
            score = 1
        elif score < -0.2:
            score = -1
        else:
            score = 0
        if state.is_lose():
            score = -1
        elif state.is_draw():
            score = 0
        elif state.is_win():
            score = 1

        if score == result:
            correct_num += 1
        # else:
        #     print(state)
        #     print("org:",org_score)
        #     print("score:",score)
        #     print("result:",result)
    print("ans:",correct_num/len(problem_list),f"({correct_num})/({len(problem_list)})")
    return correct_num/len(problem_list)    
    
# 動作確認
if __name__ == '__main__':
    evaluate_problem2()
