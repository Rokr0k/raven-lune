#pragma once

#include <string>
#include <vector>

namespace bms {
/// Object Class
class Obj {
public:
  /// Type of the objects
  enum class Type {
    /// Channel 01.
    BGM,

    /// Channel 04, 06, 07.
    BMP,

    /// Channel 11~19, 21~29, 51~59, 61~69.
    NOTE,

    /// Channel 31~39, 41~49.
    INVISIBLE,

    /// Channel D1~D9, E1~E9.
    BOMB
  } type;

  /// Position of the object
  float pos;

  /// Time it will be executed
  float time;

  union {
    /// Info for BGM Object
    struct {
      /// WAV Index
      int key;
    } bgm;

    /// Info for BMP Object
    struct {
      /// BMP Index
      int key;

      /**
       * -1: Poor BGA
       * 0: BGA Base
       * 1: BGA Layer
       */
      int layer;
    } bmp;

    /// Info for Note
    struct {
      /// Player Number
      int player;

      /// Line Number
      int line;

      /// WAV Index
      int key;

      /// Whether it is the end of a long note
      bool end;
    } note;

    /// Info for Invisible Notes and Bombs
    struct {
      /// Player Number
      int player;

      /// Line Number
      int line;

      /**
       * Invisible Note: WAV Index
       * Bomb Note: Damage, 1295 is the maximum
       */
      int key;
    } misc;
  };
};

/// Sector Class
class Sector {
public:
  /// Position of the start of the sector
  float pos;

  /// Time the sector will be started
  float time;

  /// Scroll speed
  float bpm;

  /// If false, it won't use this sector in the exactly same time
  bool inclusive;

  /**
   * Create Sector.
   * \param pos Position
   * \param time When the sector will be started
   * \param bpm Scroll speed
   * \param inclusive Whether the sector is inclusive
   */
  Sector(float pos, float time, float bpm, bool inclusive)
      : pos(pos), time(time), bpm(bpm), inclusive(inclusive) {}

  /**
   * Convert position to time.
   * \param pos Position
   * \return Time
   */
  float pos2time(float pos) const;

  /**
   * Convert time to position.
   * \param time Time
   * \return Position
   */
  float time2pos(float time) const;
};

/// Chart Class
class Chart {
public:
  /// Type of the chart
  enum class Type {
    /**
     * 7K1S
     * 16 11 12 13 14 15 18 19
     */
    Single,

    /**
     * 14K2S
     * 16 11 12 13 14 15 18 19 21 22 23 24 25 28 29 26
     */
    Dual,
  } type;

  std::string filename;

  /// Genre
  std::string genre;

  /// Title
  std::string title;

  /// Artist
  std::string artist;

  /// Subtitle
  std::string subtitle;

  /// Subartist
  std::string subartist;

  /// Path to the stagefile
  std::string stagefile;

  /// Path to the banner
  std::string banner;

  /// Play Level
  int playLevel;

  /**
   * 1: Easy
   * 2: Normal
   * 3: Hyper
   * 4: Another
   * 5: Insane
   */
  int difficulty;

  /// Total
  float total;

  /**
   * 0: Very Hard
   * 1: Hard
   * 2: Normal
   * 3: Easy
   */
  int rank;

  /// Path to the WAV files
  std::string *wavs;

  /// Path to the BMP files
  std::string *bmps;

  /// Signatures
  float *signatures;

  /// Objs
  std::vector<Obj> objs;

  /// Sectors
  std::vector<Sector> sectors;

  Chart();
  Chart(const Chart &chart);

  ~Chart();

  /**
   * Convert fraction to position.
   * \param frac Fraction
   * \return Position
   */
  float frac2pos(float frac) const;

  /**
   * Convert position to fraction.
   * \param pos Position
   * \return Fraction
   */
  float pos2frac(float pos) const;

  /**
   * Convert position to time.
   * \param pos Position
   * \return Time
   */
  float pos2time(float pos) const;

  /**
   * Convert time to position.
   * \param time Time
   * \return Position
   */
  float time2pos(float time) const;
};

/**
 * Parse .bms file.
 * \param file Path to the file
 *
 * \throw std::invalid_argument Cannot read the file
 */
Chart *parseBMS(const std::string &file);
} // namespace bms