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
Signature is used as seed for generating obfuscation keys for data. The encryption itself is just simple xor
```c
mt.seed(this->signature);
uint32_t key = mt();
for (int i = 0; i < size; i++) {
  uint8_t keyByte = ((uint8_t*)&key)[i % 4];
  buffer[i] ^= keyByte;
  key = mt();
}
```

* Signature: Signature is calculated as follow
```c
uint64_t signature = this->timestamp ^ this->highestScore | this->highestComboAchived ^ this->longestTimeAlive;
```

### References
> General setup
1. [Lazyfoo's SDL guide](https://lazyfoo.net/tutorials/SDL/index.php)
> Task and timing
2. [Timing](https://thenumb.at/cpp-course/sdl2/08/08.html#physics)
> Creating smooth color gradient
3. [Color Theory & Algorithms](https://en.wikibooks.org/wiki/Color_Theory/Algorithms#LAB)
> Bloom effect using downsampling
4. [catlikecoding's implementation](https://catlikecoding.com/unity/tutorials/advanced-rendering/bloom/)