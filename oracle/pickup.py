import random

# テキストファイルからデータを読み込み、リストに格納する関数
def read_file_to_list(file_path):
    data_list = []
    with open(file_path, 'r', encoding='utf-8') as file:
        for line in file:
            data_list.append(line.strip())  # 改行文字を削除してリストに追加
    return data_list

# リストからランダムに100個の行を選ぶ関数
def select_random_lines(data_list, num_lines):
    if num_lines >= len(data_list):
        return data_list  # リストの要素数が100以下の場合、すべての行を返す
    else:
        return random.sample(data_list, num_lines)

# 選ばれた行をテキストファイルに出力する関数
def write_selected_lines_to_file(selected_lines, output_file_path):
    with open(output_file_path, 'w', encoding='utf-8') as file:
        file.write('\n'.join(selected_lines))

# メインプログラム
if __name__ == "__main__":
    input_file_path = "all.txt"  # 入力テキストファイルのパスを指定してください
    output_file_path = "test.txt"  # 出力テキストファイルのパスを指定してください
    num_lines_to_select = 10000  # ランダムに選ぶ行の数を指定してください

    # テキストファイルからデータを読み込み、リストに格納
    data_list = read_file_to_list(input_file_path)

    # リストからランダムに100個の行を選ぶ
    selected_lines = select_random_lines(data_list, num_lines_to_select)

    # 選ばれた行をテキストファイルに出力
    write_selected_lines_to_file(selected_lines, output_file_path)
