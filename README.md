# fruit-game

## A game project for my Advance Programming class @ UET-VNU

### Thông tin tác giả
* Họ và tên: Lê Thái Vinh
* Mã sinh viên: 24021678
* Lớp: K69I - CS2

### Giới thiệu game
Game lấy cảm hứng từ trò chơi Fruit Ninja. Bạn sẽ được cho 3 mạng mỗi lượt chơi và phải cố đạt điểm cao nhất và không làm lỡ 1 quả nào.
Trong quá trình chơi sẽ có buff tùy vào số combo mà người chơi đạt được

## Ngưỡng điểm 
Trong khoảng từ 9.5-10

### Design pattern
Game sử dụng singleton-pattern, chia nhỏ ra thành các manager.

### Điểm đáng chú ý
1. On-Demand Texture streaming and easy to handle texture library
2. Game-data encryption, preventing player from tampering with data
3. Custom scene/ui framework for easier handling
4. Timing/Task-ing system for running repeated, timer

### File-save
* Structure:
```c
struct FilesaveStructure {
	uint64_t timestamp; // thời gian lưu file
	uint64_t signature; // signature kiểm tra data
	uint32_t dataSize;
	uint8_t* data; // game encrypted data
};
struct GameDataStructure {
	uint32_t highestScore;
	uint32_t highestCombo;
	uint64_t longestTimeAlive;
	std::string bladeColorId; //null-terminated string
};
```

* Encryption:
Timestamp is used as seed for generating obfuscation keys for data. The encryption itself is just simple xor
```c
//Encryption
randomSeed.seed(this->timestamp);
uint64_t key = randomSeed();
for (int i = 0; i < size; i++) {
	//using byte at byte n % 8 of the seed and xor with byte n of encrypted buffer
	uint8_t keyByte = ((uint8_t*)&key)[i % 8]; 
	buffer[i] ^= keyByte;
	//regenerate seed again
	key = randomSeed();
}
```

* Signature: Signature is a [FNV hashed](https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function) game data string, used for validating game data
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
1. *Lazy Foo' Productions*. https://lazyfoo.net/tutorials/SDL/index.php
2. *Timing, physiscs..*. https://thenumb.at/cpp-course/sdl2/08/08.html
3. *Bresenham's line algorithm* https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
4. *Bresenham's line algorithm implementation by Geeksforgeeks* https://www.geeksforgeeks.org/bresenhams-line-generation-algorithm/
5. *Color Theory/Algorithms* https://en.wikibooks.org/wiki/Color_Theory/Algorithms#LAB
6. *Catlike Coding's Bloom implementation* https://catlikecoding.com/unity/tutorials/advanced-rendering/bloom/
