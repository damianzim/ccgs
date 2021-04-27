#!/usr/bin/env python3

from collections import OrderedDict
from dataclasses import dataclass
import statistics as st
import sys

def trait_keys():
  keys = OrderedDict()
  keys['sw'] = 'Swift'
  keys['sy'] = 'Symbiotic'
  keys['po'] = 'Poisonous'
  keys['em'] = 'Empowering'
  keys['sa'] = 'Sabotaging'
  keys['su'] = 'Supporting'
  return keys

def attr_keys():
  keys = OrderedDict()
  keys['W'] = 'Water'
  keys['F'] = 'Fire'
  keys['N'] = 'Nature'
  return keys

@dataclass
class CardsData:
  strength = []
  attrs = {'W': [], 'F': [], 'N': []}
  traits = {'sw': 0, 'sy': 0, 'po': 0, 'em': 0, 'sa': 0, 'su': 0}

  def __len__(self):
    return len(self.strength)

  @staticmethod
  def parse_log_item(item, dtype=int):
    name, value = item.split('=', 1)
    return name, dtype(value)

  def append(self, line):
    class TraitVal:
      def __init__(self, value):
        if value not in ['Y', 'N']:
          raise ValueError('Expected Y or N for TraitVal')
        self.value = value == 'Y'
      def __int__(self):
        return int(self.value)
    self.strength.append(CardsData.parse_log_item(line[0], float)[1])
    line = line[1:]
    for item in line[:3]:
      name, value = CardsData.parse_log_item(item)
      self.attrs[name].append(value)
    line = line[3:]
    for item in line:
      name, value = CardsData.parse_log_item(item, TraitVal)
      self.traits[name] += int(value)

def analyse(data):
  total = len(data)
  print('Total', total, 'cards')
  print()
  print('Strength        SD')
  print('----------- ------')
  print(f'{st.mean(data.strength):<10.2f}   {st.pstdev(data.strength):5.2f}')
  print()
  print('Attribute     Mean     SD')
  print('----------- ------ ------')
  for key, name in attr_keys().items():
    attrs = data.attrs[key]
    print(f'{name:10s}   {st.mean(attrs):5.2f}  {st.pstdev(attrs):5.2f}')
  print()
  print('Trait        Count       %')
  print('----------- ------ -------')
  for key, name in trait_keys().items():
    traits = data.traits[key]
    print(f'{name:10s}   {traits:5}  {traits/total*100:6.02f}')

def main():
  data = CardsData()
  for line in sys.stdin:
    line = line.strip()
    if not line:
      break
    line = line.split()[3:]
    if len(line) == 0 or not line[0].startswith('ST='):
      continue
    data.append(line)
  if len(data):
    analyse(data)

if __name__ == '__main__':
  sys.exit(main())
