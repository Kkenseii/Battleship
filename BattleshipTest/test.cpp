#include "pch.h"
#include "position.h"
#include "ship.h"
#include "gamefield.h"
#include "gamelib.h"
#include "playerlib.h"

#include <sstream>
#include <stdexcept>
#include <string>
#include <iostream>



//Position

TEST(PositionTest, DefaultConstructor_IsTopLeftCorner) {
    Position pos;
    EXPECT_EQ(pos.row(), 1);
    EXPECT_EQ(pos.col(), 1);
}

TEST(PositionTest, ParamConstructor_ValidValues) {
    Position pos(5, 7);
    EXPECT_EQ(pos.row(), 5);
    EXPECT_EQ(pos.col(), 7);
}

TEST(PositionTest, ParamConstructor_RowTooLow_Throws) {
    EXPECT_THROW(Position(0, 5), std::logic_error);
}

TEST(PositionTest, ParamConstructor_RowTooHigh_Throws) {
    EXPECT_THROW(Position(11, 5), std::logic_error);
}

TEST(PositionTest, ParamConstructor_ColTooLow_Throws) {
    EXPECT_THROW(Position(5, 0), std::logic_error);
}

TEST(PositionTest, ParamConstructor_ColTooHigh_Throws) {
    EXPECT_THROW(Position(5, 11), std::logic_error);
}

TEST(PositionTest, ParamConstructor_BoundaryValuesAreValid) {
    EXPECT_NO_THROW(Position(1, 1));
    EXPECT_NO_THROW(Position(10, 10));
}

TEST(PositionTest, CopyConstructor_CopiesValues) {
    Position original(3, 9);
    Position copy(original);
    EXPECT_EQ(copy.row(), 3);
    EXPECT_EQ(copy.col(), 9);
}

TEST(PositionTest, StringConstructor_ValidFormat) {
    Position pos("(3, 4)");
    EXPECT_EQ(pos.row(), 3);
    EXPECT_EQ(pos.col(), 4);
}

TEST(PositionTest, StringConstructor_TrimsWhitespace) {
    Position pos("(  6 ,   2 )");
    EXPECT_EQ(pos.row(), 6);
    EXPECT_EQ(pos.col(), 2);
}

TEST(PositionTest, StringConstructor_MissingParenthesis_Throws) {
    EXPECT_THROW(Position("3, 4"), std::logic_error);
}

TEST(PositionTest, StringConstructor_MissingComma_Throws) {
    EXPECT_THROW(Position("(3 4)"), std::logic_error);
}

TEST(PositionTest, StringConstructor_OutOfRangeValues_Throws) {
    EXPECT_THROW(Position("(11, 4)"), std::logic_error);
}

TEST(PositionTest, StringConstructor_NonNumeric_Throws) {
    EXPECT_THROW(Position("(a, b)"), std::logic_error);
}

TEST(PositionTest, RowSetter_ValidValue) {
    Position pos;
    pos.row(8);
    EXPECT_EQ(pos.row(), 8);
}

TEST(PositionTest, RowSetter_InvalidValue_Throws) {
    Position pos;
    EXPECT_THROW(pos.row(0), std::logic_error);
    EXPECT_THROW(pos.row(11), std::logic_error);
}

TEST(PositionTest, ColSetter_ValidValue) {
    Position pos;
    pos.col(6);
    EXPECT_EQ(pos.col(), 6);
}

TEST(PositionTest, ColSetter_InvalidValue_Throws) {
    Position pos;
    EXPECT_THROW(pos.col(0), std::logic_error);
    EXPECT_THROW(pos.col(11), std::logic_error);
}

TEST(PositionTest, ToString_FormatsCorrectly) {
    Position pos(3, 4);
    EXPECT_EQ(to_string(pos), "(3, 4)");
}

TEST(PositionTest, ToString_DoubleDigitValues) {
    Position pos(10, 10);
    EXPECT_EQ(to_string(pos), "(10, 10)");
}

TEST(PositionTest, Parse_FriendFunction_Works) {
    Position pos = parse("(2, 9)");
    EXPECT_EQ(pos.row(), 2);
    EXPECT_EQ(pos.col(), 9);
}

TEST(PositionTest, Parse_InvalidString_Throws) {
    EXPECT_THROW(parse("nonsense"), std::logic_error);
}

TEST(PositionTest, ToString_ParseRoundTrip) {
    Position original(4, 8);
    Position roundTripped = parse(to_string(original));
    EXPECT_EQ(roundTripped.row(), original.row());
    EXPECT_EQ(roundTripped.col(), original.col());
}

//Gamefield

TEST(GameFieldTest, DefaultConstructor_IsTenByTenAndEmpty) {
    GameField field;
    EXPECT_EQ(field.get(1, 'A'), ' ');
    EXPECT_EQ(field.get(10, 'J'), ' ');
}

TEST(GameFieldTest, DefaultConstructor_OutOfRangeRowThrows) {
    GameField field;
    EXPECT_THROW(field.get(11, 'A'), std::logic_error);
    EXPECT_THROW(field.set(0, 'A'), std::logic_error);
}

TEST(GameFieldTest, DefaultConstructor_OutOfRangeColumnThrows) {
    GameField field;
    EXPECT_THROW(field.get(1, 'K'), std::logic_error);
    EXPECT_THROW(field.get(1, '1'), std::logic_error);
}

TEST(GameFieldTest, SizeConstructor_ValidDimensions) {
    GameField field(5, 7);
    EXPECT_NO_THROW(field.get(5, 'G'));
    EXPECT_THROW(field.get(6, 'A'), std::logic_error);
    EXPECT_THROW(field.get(1, 'H'), std::logic_error);
}

TEST(GameFieldTest, SizeConstructor_ZeroThrows) {
    EXPECT_THROW(GameField(0, 5), std::logic_error);
    EXPECT_THROW(GameField(5, 0), std::logic_error);
}

TEST(GameFieldTest, SizeConstructor_NegativeThrows) {
    EXPECT_THROW(GameField(-1, 5), std::logic_error);
}

TEST(GameFieldTest, SizeConstructor_TooLargeThrows) {
    EXPECT_THROW(GameField(26, 5), std::logic_error);
    EXPECT_THROW(GameField(5, 26), std::logic_error);
}

TEST(GameFieldTest, SizeConstructor_BoundaryTwentyFiveIsValid) {
    EXPECT_NO_THROW(GameField(25, 25));
}

TEST(GameFieldTest, ArrayConstructor_CopiesGivenData) {
    char** raw = new char* [2];
    raw[0] = new char[2];
    raw[1] = new char[2];
    raw[0][0] = '*'; raw[0][1] = ' ';
    raw[1][0] = ' '; raw[1][1] = '*';

    GameField field(raw, 2, 2);
    EXPECT_EQ(field.get(1, 'A'), '*');
    EXPECT_EQ(field.get(1, 'B'), ' ');
    EXPECT_EQ(field.get(2, 'A'), ' ');
    EXPECT_EQ(field.get(2, 'B'), '*');

    delete[] raw[0];
    delete[] raw[1];
    delete[] raw;
}

TEST(GameFieldTest, ArrayConstructor_IsDeepCopy) {
    char** raw = new char* [1];
    raw[0] = new char[1];
    raw[0][0] = '*';

    GameField field(raw, 1, 1);
    raw[0][0] = ' '; 
    EXPECT_EQ(field.get(1, 'A'), '*'); 

    delete[] raw[0];
    delete[] raw;
}

TEST(GameFieldTest, ArrayConstructor_InvalidSizeThrows) {
    char** raw = new char* [1];
    raw[0] = new char[1];
    raw[0][0] = ' ';
    EXPECT_THROW(GameField(raw, 0, 1), std::logic_error);
    delete[] raw[0];
    delete[] raw;
}

TEST(GameFieldTest, CopyConstructor_IsDeepCopy) {
    GameField original(3, 3);
    original.set(1, 'A');

    GameField copy(original);
    copy.set(2, 'B');

    EXPECT_EQ(original.get(1, 'A'), '*');
    EXPECT_EQ(original.get(2, 'B'), ' '); 
    EXPECT_EQ(copy.get(1, 'A'), '*');
    EXPECT_EQ(copy.get(2, 'B'), '*');
}

TEST(GameFieldTest, Set_MarksCellWithAsterisk) {
    GameField field;
    field.set(4, 'C');
    EXPECT_EQ(field.get(4, 'C'), '*');
}

TEST(GameFieldTest, Set_LowercaseColumnWorks) {
    GameField field;
    field.set(4, 'c');
    EXPECT_EQ(field.get(4, 'C'), '*');
}

TEST(GameFieldTest, Set_InvalidPositionThrows) {
    GameField field;
    EXPECT_THROW(field.set(0, 'A'), std::logic_error);
    EXPECT_THROW(field.set(1, '$'), std::logic_error);
}

TEST(GameFieldTest, Get_InvalidPositionThrows) {
    GameField field;
    EXPECT_THROW(field.get(-1, 'A'), std::logic_error);
}

TEST(GameFieldTest, ToString_EmptyFieldFormat) {
    GameField field(3, 3);
    std::string expected =
        "  |A B C|\n"
        "  +-----+\n"
        "1 | | | |\n"
        "2 | | | |\n"
        "3 | | | |\n"
        "  +-----+";
    EXPECT_EQ(to_string(field), expected);
}

TEST(GameFieldTest, ToString_ShowsPlacedShips) {
    GameField field(3, 3);
    field.set(2, 'B');
    std::string expected =
        "  |A B C|\n"
        "  +-----+\n"
        "1 | | | |\n"
        "2 | |*| |\n"
        "3 | | | |\n"
        "  +-----+";
    EXPECT_EQ(to_string(field), expected);
}

TEST(GameFieldTest, ToStringHide_HidesShipsWhenTrue) {
    GameField field(3, 3);
    field.set(2, 'B');
    std::string expected =
        "  |A B C|\n"
        "  +-----+\n"
        "1 | | | |\n"
        "2 | | | |\n"
        "3 | | | |\n"
        "  +-----+";
    EXPECT_EQ(to_string(field, true), expected);
}

TEST(GameFieldTest, ToStringHide_ShowsShipsWhenFalse) {
    GameField field(3, 3);
    field.set(2, 'B');
    std::string expected =
        "  |A B C|\n"
        "  +-----+\n"
        "1 | | | |\n"
        "2 | |*| |\n"
        "3 | | | |\n"
        "  +-----+";
    EXPECT_EQ(to_string(field, false), expected);
}

TEST(GameFieldTest, ToString_TenColumnHeaderMatchesLetters) {
    GameField field; // default 10x10
    std::string result = to_string(field);
    EXPECT_NE(result.find("A B C D E F G H I J"), std::string::npos);
}

TEST(GameFieldTest, ToString_RowLabelPaddingForDoubleDigitRow) {
    GameField field; 
    std::string result = to_string(field);
    EXPECT_NE(result.find("10|"), std::string::npos);
    EXPECT_EQ(result.find("10 |"), std::string::npos);
}

//Ship

TEST(ShipTest, Constructor_ValidHorizontalShip) {
    Ship ship(3, Position(4, 3), Horizontal);
    EXPECT_EQ(ship.size(), 3);
    EXPECT_EQ(ship.direction(), Horizontal);
    EXPECT_EQ(ship.row(), 4);
    EXPECT_EQ(ship.col(), 3);
}

TEST(ShipTest, Constructor_ValidVerticalShip) {
    Ship ship(2, Position(5, 5), Vertical);
    EXPECT_EQ(ship.direction(), Vertical);
}

TEST(ShipTest, Constructor_SizeTooSmall_Throws) {
    EXPECT_THROW(Ship(0, Position(1, 1), Horizontal), std::logic_error);
}

TEST(ShipTest, Constructor_SizeTooLarge_Throws) {
    EXPECT_THROW(Ship(5, Position(1, 1), Horizontal), std::logic_error);
}

TEST(ShipTest, Constructor_HorizontalOutOfBounds_Throws) {
    EXPECT_THROW(Ship(4, Position(1, 8), Horizontal), std::logic_error);
}

TEST(ShipTest, Constructor_VerticalOutOfBounds_Throws) {
    EXPECT_THROW(Ship(4, Position(8, 1), Vertical), std::logic_error);
}

TEST(ShipTest, Constructor_BoundaryFitsExactly) {
    EXPECT_NO_THROW(Ship(4, Position(1, 7), Horizontal)); // columns 7..10
    EXPECT_NO_THROW(Ship(4, Position(7, 1), Vertical));   // rows 7..10
}

TEST(ShipTest, PartialConstructor_DefaultsToHorizontal) {
    Ship ship(3, Position(2, 2));
    EXPECT_EQ(ship.direction(), Horizontal);
}

TEST(ShipTest, PartialConstructor_OutOfBounds_Throws) {
    EXPECT_THROW(Ship(4, Position(1, 9)), std::logic_error);
}

TEST(ShipTest, CharConstructor_UppercaseHorizontal) {
    Ship ship(3, 'H', 4, 'C');
    EXPECT_EQ(ship.size(), 3);
    EXPECT_EQ(ship.direction(), Horizontal);
    EXPECT_EQ(ship.row(), 4);
    EXPECT_EQ(ship.col(), 3);
}

TEST(ShipTest, CharConstructor_LowercaseVertical) {
    Ship ship(2, 'v', 3, 'b');
    EXPECT_EQ(ship.direction(), Vertical);
    EXPECT_EQ(ship.row(), 3);
    EXPECT_EQ(ship.col(), 2);
}

TEST(ShipTest, CharConstructor_InvalidDirection_Throws) {
    EXPECT_THROW(Ship(2, 'X', 1, 'A'), std::logic_error);
}

TEST(ShipTest, CharConstructor_InvalidColumnLetter_Throws) {
    EXPECT_THROW(Ship(2, 'H', 1, '$'), std::logic_error);
    EXPECT_THROW(Ship(2, 'H', 1, 'K'), std::logic_error);
}

TEST(ShipTest, CharConstructor_InvalidRow_Throws) {
    EXPECT_THROW(Ship(2, 'H', 0, 'A'), std::logic_error);
    EXPECT_THROW(Ship(2, 'H', 11, 'A'), std::logic_error);
}

TEST(ShipTest, CharConstructor_CollisionThrows) {
    EXPECT_THROW(Ship(4, 'H', 1, 'J'), std::logic_error);
}

TEST(ShipTest, DefaultConstructor_IsDeleted) {
    EXPECT_FALSE(std::is_default_constructible<Ship>::value);
}

TEST(ShipTest, CopyConstructor_IsDeleted) {
    EXPECT_FALSE(std::is_copy_constructible<Ship>::value);
}

TEST(ShipTest, Rotate_TogglesDirection) {
    Ship ship(2, Position(3, 3), Horizontal);
    ship.rotate();
    EXPECT_EQ(ship.direction(), Vertical);
    ship.rotate();
    EXPECT_EQ(ship.direction(), Horizontal);
}

TEST(ShipTest, Rotate_KeepsPositionAndSize) {
    Ship ship(3, Position(4, 3), Horizontal);
    ship.rotate();
    EXPECT_EQ(ship.size(), 3);
    EXPECT_EQ(ship.row(), 4);
    EXPECT_EQ(ship.col(), 3);
}

TEST(ShipTest, Rotate_ThrowsWhenResultWouldCollide) {
    Ship ship(4, Position(8, 1), Horizontal);
    EXPECT_THROW(ship.rotate(), std::logic_error);
}

TEST(ShipTest, Rotate_DoesNotChangeDirectionOnFailedRotation) {
    Ship ship(4, Position(8, 1), Horizontal);
    try {
        ship.rotate();
    }
    catch (const std::logic_error&) {
    }
    EXPECT_EQ(ship.direction(), Horizontal);
}

namespace {

    template <typename Action>
    std::string capture_stdout(Action action) {
        std::ostringstream buffer;
        std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
        action();
        std::cout.rdbuf(old);
        return buffer.str();
    }

    void place_full_fleet(Player& player) {
        player.set_ship(Ship(4, 'H', 1, 'A'));
        player.set_ship(Ship(3, 'H', 3, 'A'));
        player.set_ship(Ship(3, 'H', 5, 'A'));
        player.set_ship(Ship(2, 'H', 7, 'A'));
        player.set_ship(Ship(2, 'H', 9, 'A'));
        player.set_ship(Ship(2, 'H', 1, 'F'));
        player.set_ship(Ship(1, 'H', 3, 'F'));
        player.set_ship(Ship(1, 'H', 5, 'F'));
        player.set_ship(Ship(1, 'H', 7, 'F'));
        player.set_ship(Ship(1, 'H', 9, 'F'));
    }

}

TEST(PlayerTest, DefaultConstructor_IsNotReady) {
    Player player;
    EXPECT_FALSE(player.check_ready());
}

TEST(PlayerTest, DefaultConstructor_HasNoShipsSoReportsLose) {
    Player player;
    EXPECT_TRUE(player.check_lose());
}

TEST(PlayerTest, SetShip_FullFleetBecomesReady) {
    Player player;
    place_full_fleet(player);
    EXPECT_TRUE(player.check_ready());
}

TEST(PlayerTest, SetShip_FullFleetIsNotLost) {
    Player player;
    place_full_fleet(player);
    EXPECT_FALSE(player.check_lose());
}

TEST(PlayerTest, SetShip_ExceedingMaxCountForSizeThrows) {
    Player player;
    player.set_ship(Ship(1, 'H', 1, 'A'));
    player.set_ship(Ship(1, 'H', 3, 'A'));
    player.set_ship(Ship(1, 'H', 5, 'A'));
    player.set_ship(Ship(1, 'H', 7, 'A'));
    EXPECT_THROW(player.set_ship(Ship(1, 'H', 9, 'A')), std::logic_error);
}

TEST(PlayerTest, SetShip_OverlappingShipThrows) {
    Player player;
    player.set_ship(Ship(3, 'H', 1, 'A')); // covers A1,B1,C1
    EXPECT_THROW(player.set_ship(Ship(2, 'V', 1, 'B')), std::logic_error);
}

TEST(PlayerTest, SetShip_AdjacentTouchingShipThrows) {
    Player player;
    player.set_ship(Ship(2, 'H', 1, 'A')); // covers A1,B1
    EXPECT_THROW(player.set_ship(Ship(1, 'H', 1, 'C')), std::logic_error);
}

TEST(PlayerTest, SetAction_MissOnEmptyCellReturnsMissed) {
    Player player;
    player.set_ship(Ship(2, 'H', 1, 'A')); // A1,B1
    State result = player.set_action(5, 'E');
    EXPECT_EQ(result, State::Missed);
}

TEST(PlayerTest, SetAction_HitOnMultiCellShipReturnsHitBeforeSunk) {
    Player player;
    player.set_ship(Ship(2, 'H', 1, 'A')); // A1,B1
    State result = player.set_action(1, 'A');
    EXPECT_EQ(result, State::Hit);
}

TEST(PlayerTest, SetAction_SinkingSingleCellShipReturnsBoatDestroyed) {
    Player player;
    player.set_ship(Ship(1, 'H', 1, 'A'));
    State result = player.set_action(1, 'A');
    EXPECT_EQ(result, State::BoatDestroyed);
}

TEST(PlayerTest, SetAction_SinkingTwoCellShipReturnsDestroyersDestroyed) {
    Player player;
    player.set_ship(Ship(2, 'H', 1, 'A')); // A1,B1
    player.set_action(1, 'A');
    State result = player.set_action(1, 'B');
    EXPECT_EQ(result, State::DestroyersDestroyed);
}

TEST(PlayerTest, SetAction_SinkingThreeCellShipReturnsCruisersDestroyed) {
    Player player;
    player.set_ship(Ship(3, 'H', 1, 'A')); // A1,B1,C1
    player.set_action(1, 'A');
    player.set_action(1, 'B');
    State result = player.set_action(1, 'C');
    EXPECT_EQ(result, State::CruisersDestroyed);
}

TEST(PlayerTest, SetAction_SinkingFourCellShipReturnsBattleshipDestroyed) {
    Player player;
    player.set_ship(Ship(4, 'H', 1, 'A')); // A1,B1,C1,D1
    player.set_action(1, 'A');
    player.set_action(1, 'B');
    player.set_action(1, 'C');
    State result = player.set_action(1, 'D');
    EXPECT_EQ(result, State::BattleshipDestroyed);
}

TEST(PlayerTest, SetAction_RepeatedShotOnSameCellThrows) {
    Player player;
    player.set_ship(Ship(1, 'H', 1, 'A'));
    player.set_action(1, 'A');
    EXPECT_THROW(player.set_action(1, 'A'), std::logic_error);
}

TEST(PlayerTest, SetAction_RepeatedMissThrows) {
    Player player;
    player.set_action(5, 'E');
    EXPECT_THROW(player.set_action(5, 'E'), std::logic_error);
}

TEST(PlayerTest, SetAction_OutOfRangePositionThrows) {
    Player player;
    EXPECT_THROW(player.set_action(11, 'A'), std::logic_error);
    EXPECT_THROW(player.set_action(1, 'K'), std::logic_error);
}

TEST(PlayerTest, CheckLose_TrueOnceAllPlacedShipsAreSunk) {
    Player player;
    player.set_ship(Ship(1, 'H', 1, 'A'));
    player.set_ship(Ship(1, 'H', 3, 'A'));
    EXPECT_FALSE(player.check_lose());
    player.set_action(1, 'A');
    EXPECT_FALSE(player.check_lose());
    player.set_action(3, 'A');
    EXPECT_TRUE(player.check_lose());
}

TEST(PlayerTest, ShowField_OutputsBoardAndShipsLeftLine) {
    Player player;
    player.set_ship(Ship(1, 'H', 1, 'A'));

    std::string output = capture_stdout([&]() { player.show_field(); });

    EXPECT_NE(output.find("|A B C D E F G H I J|"), std::string::npos);
    EXPECT_NE(output.find("Ships Left:"), std::string::npos);
    EXPECT_NE(output.find("* - 1 ** - 0 *** - 0 **** - 0"), std::string::npos);
}

TEST(PlayerTest, ShowField_HideShipsOmitsAsterisk) {
    Player player;
    player.set_ship(Ship(1, 'H', 1, 'A'));

    std::string shown = capture_stdout([&]() { player.show_field(false); });
    std::string hidden = capture_stdout([&]() { player.show_field(true); });

    EXPECT_NE(shown.find('*'), std::string::npos);
    EXPECT_EQ(hidden.find("1 |*|"), std::string::npos);
}