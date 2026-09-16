import random
import time

alphabet = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
            'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x']

removed_letters = []

for i in range(24):
    random_index = random.randint(0,len(alphabet)-1)
    print(f"\ra{alphabet[random_index]}",end="", flush=True)
    removed_letters.append('a' + alphabet[random_index])
    alphabet.pop(random_index)
    time.sleep(0.5)

input("\nAppuyez sur Entrée pour continuer...")
print(removed_letters)
print("\nTerminé !")
