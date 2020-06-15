import json
import argparse

def  Parser():
    pass

json_name = './gcd_graph.json'

def val_sort(json_dict):
    items = json_dict.items()
    ret = [(v[1],v[0]) for v in items]
    ret.sort()
    return ret

if __name__ == '__main__':
    with open(json_name,'r') as load_f:
        json_dict = json.load(load_f)
    
    # print(load_dict.keys())
    # print(degree)
    degree = {i:0 for i in json_dict.keys()}
    for i in json_dict.keys():
        # print(i)
        for j in json_dict[i]["sinks"]:
            degree[j] += 1
    items = val_sort(degree)
    print(items)

