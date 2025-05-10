# fruit-game

## A game project for my Advance Programming class @ UET-VNU

### File-save
* Structure:
```c
struct FilesaveStructure{
  uint64_t timestamp; // Thời gian bắt đầu save file
  uint64_t signature; // Signature kiểm tra nếu kỉ lục có thật sự đúng không
  uint32_t fileSize;
  uint8_t* data;
};

struct GameData{
  uint32_t highestScore;
  uint32_t highestCombo;
  uint64_t longestTimeAlive;
};
```

* Encryption:
Timestamp is used as seed for generating obfuscation keys for data. The encryption itself is just simple xor
```c
//Encryption
randomSeed.seed(this->timestamp);
uint64_t key = randomSeed();
for (int i = 0; i < size; i++) {
	uint8_t keyByte = ((uint8_t*)&key)[i % 8];
	buffer[i] ^= keyByte;
	key = randomSeed();
}
```

* Signature: Signature is a [FNV hashed](https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function) game data string
```c
inline const uint32_t HashGameDataStringFN(string gameDataStr) {

	uint32_t hash = 0x811c9dc5;
	uint32_t prime = 0x1000193;

	for (int i = 0; i < gameDataStr.length(); ++i) {
		uint8_t value = gameDataStr[i];
		hash = hash ^ value;
		hash *= prime;
	}

	return hash;
}

string gameDataString = to_string(highestScore) + ":" + to_string(highestComboAchieved) + ":" + to_string(longestTimeAlive)
uint64_t signature = HashGameDataStringFN(gameDataString);
```

### References
> General setup
1. [Lazyfoo's SDL guide](https://lazyfoo.net/tutorials/SDL/index.php)
> Task and timing
2. [Timing](https://thenumb.at/cpp-course/sdl2/08/08.html#physics)
> Line algorithm
3. [Bresenham's line algorithm](https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm)
4. [GeeksforGeeks's line algorithm implementation](https://www.geeksforgeeks.org/bresenhams-line-generation-algorithm/)
> Creating smooth color gradient
5. [Color Theory & Algorithms](https://en.wikibooks.org/wiki/Color_Theory/Algorithms#LAB)
> Bloom effect using downsampling
6. [catlikecoding's implementation](https://catlikecoding.com/unity/tutorials/advanced-rendering/bloom/)
