// Define Input Button
#define BUTTON_0 13
#define BUTTON_1 12
#define BUTTON_2 11
#define BUTTON_3 10
#define BUTTON_4 9
#define BUTTON_5 8
#define BUTTON_6 7

// Define Out
#define TX_OUT 1

// Define State
#define col0_p1 0
#define col1_p1 1
#define col2_p1 2
#define col3_p1 3
#define col4_p1 4
#define col5_p1 5
#define col6_p1 6
#define col0_p2 7
#define col1_p2 8
#define col2_p2 9
#define col3_p2 10
#define col4_p2 11
#define col5_p2 12
#define col6_p2 13

typedef struct s_fsm
{
	int	col;
	int	player;
	int	time;
	int	next_st[8];
}	t_fsm;

static int	board[6][7] = {
	{0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,},
};

t_fsm	FSM[14] = {
	{0, 1, 1500, {col0_p1, col0_p2, col1_p2, col2_p2, col3_p2, col4_p2, col5_p2, col6_p2, }}, 	// col0_p1
	{1, 1, 1500, {col1_p1, col0_p2, col1_p2, col2_p2, col3_p2, col4_p2, col5_p2, col6_p2, }},	// col1_p1
	{2, 1, 1500, {col2_p1, col0_p2, col1_p2, col2_p2, col3_p2, col4_p2, col5_p2, col6_p2, }},	// col2_p1
	{3, 1, 1500, {col3_p1, col0_p2, col1_p2, col2_p2, col3_p2, col4_p2, col5_p2, col6_p2, }},	// col3_p1
	{4, 1, 1500, {col4_p1, col0_p2, col1_p2, col2_p2, col3_p2, col4_p2, col5_p2, col6_p2, }},	// col4_p1
	{5, 1, 1500, {col5_p1, col0_p2, col1_p2, col2_p2, col3_p2, col4_p2, col5_p2, col6_p2, }},	// col5_p1
	{6, 1, 1500, {col6_p1, col0_p2, col1_p2, col2_p2, col3_p2, col4_p2, col5_p2, col6_p2, }},	// col6_p1

	{0, 2, 1500, {col0_p2, col0_p1, col1_p1, col2_p1, col3_p1, col4_p1, col5_p1, col6_p1, }}, 	// col0_p2
	{1, 2, 1500, {col1_p2, col0_p1, col1_p1, col2_p1, col3_p1, col4_p1, col5_p1, col6_p1, }}, 	// col1_p2
	{2, 2, 1500, {col2_p2, col0_p1, col1_p1, col2_p1, col3_p1, col4_p1, col5_p1, col6_p1, }}, 	// col2_p2
	{3, 2, 1500, {col3_p2, col0_p1, col1_p1, col2_p1, col3_p1, col4_p1, col5_p1, col6_p1, }}, 	// col3_p2
	{4, 2, 1500, {col4_p2, col0_p1, col1_p1, col2_p1, col3_p1, col4_p1, col5_p1, col6_p1, }}, 	// col4_p2
	{5, 2, 1500, {col5_p2, col0_p1, col1_p1, col2_p1, col3_p1, col4_p1, col5_p1, col6_p1, }}, 	// col5_p2
	{6, 2, 1500, {col6_p2, col0_p1, col1_p1, col2_p1, col3_p1, col4_p1, col5_p1, col6_p1, }}, 	// col6_p2
};

static int	ST = col0_p2;
static int	last_ST = col0_p2;
static int	winner = 0;

void	setup(void)
{
	// Setup Button
	pinMode(BUTTON_0, INPUT_PULLUP);
	pinMode(BUTTON_1, INPUT_PULLUP);
	pinMode(BUTTON_2, INPUT_PULLUP);
	pinMode(BUTTON_3, INPUT_PULLUP);
	pinMode(BUTTON_4, INPUT_PULLUP);
	pinMode(BUTTON_5, INPUT_PULLUP);
	pinMode(BUTTON_6, INPUT_PULLUP);

	// Serial Display
	Serial.begin(9600);
	display_board_serial();
}

void	loop(void)
{
	int	input;

	// Handle no state change
	if (last_ST != ST)
	{
		put_coin(FSM[ST].col, FSM[ST].player);
		display_board_serial();
		winner = check_win();
		last_ST = ST;
	}
	// Handle win
	if (winner)
	{
		Serial.print("Winner is Player: ");
		Serial.println(winner);
		winner = 0;
		set_zero_board();
		display_board_serial();
	}
	delay(FSM[ST].time);

	// Input State
	input = get_input();
	ST = FSM[ST].next_st[input];
}

int	get_input(void)
{
	int	input0;
	int	input1;
	int	input2;
	int	input3;
	int	input4;
	int	input5;
	int	input6;
	int	input;

	input0 = !(digitalRead(BUTTON_0)) * 1;
	input1 = !(digitalRead(BUTTON_1)) * 2;
	input2 = !(digitalRead(BUTTON_2)) * 4;
	input3 = !(digitalRead(BUTTON_3)) * 8;
	input4 = !(digitalRead(BUTTON_4)) * 16;
	input5 = !(digitalRead(BUTTON_5)) * 32;
	input6 = !(digitalRead(BUTTON_6)) * 64;

	input = input0 + input1 + input2 + input3 + input4 + input5 + input6;
	if (input == 1)
		return (1);
	else if (input == 2)
		return (2);
	else if (input == 4)
		return (3);
	else if (input == 8)
		return (4);
	else if (input == 16)
		return (5);
	else if (input == 32)
		return (6);
	else if (input == 64)
		return (7);
	else
		return (0);
}

void	put_coin(int col, int player)
{
    int	i;

	i = 5;
    while (i >= 0)
	{
		if (board[i][col] == 0)
		{
			board[i][col] = player;
			return ;
		}
		--i;
	}
}

void	display_board_serial(void)
{
	Serial.println("Board Array:");

	for (int row = 0; row < 6; row++)
	{
		for (int col = 0; col < 7; col++)
		{
			Serial.print(board[row][col]);
			if (col < 6)
				Serial.print(",");
		}
		Serial.println();
	}
	Serial.println();
}

int	check_win(void)
{
	int	row;
	int	col;
	int	player;

	row = 5;
	player = 1;
	while (row >= 0)
	{
		col = 6;
		while (col >= 0)
		{
			if (board[row][col])
			{
				player = board[row][col];
				if (check_win_up(row, col, player) || check_win_left(row, col, player)
					|| check_win_right(row, col, player) || check_win_up_left(row, col, player)
					|| check_win_up_right(row, col, player))
					return (player);
			}
			--col;
		}
		--row;
	}
	return (0);
}

int	check_win_up(int row, int col, int player)
{
	int	i;

	if (row < 3)
		return (0);
	i = 0;
	while (i < 4)
	{
		if (board[row - i][col] != player)
			return (0);
		++i;
	}
	return (player);
}

int	check_win_left(int row, int col, int player)
{
	int	i;

	if (col < 3)
		return (0);
	i = 0;
	while (i < 4)
	{
		if (board[row][col - i] != player)
			return (0);
		++i;
	}
	return (player);
}
int	check_win_right(int row, int col, int player)
{
	int	i;

	if (col > 3)
		return (0);
	i = 0;
	while (i < 4)
	{
		if (board[row][col + i] != player)
			return (0);
		++i;
	}
	return (player);
}

int	check_win_up_left(int row, int col, int player)
{
	int	i;

	if (row < 3 || col < 3)
		return (0);
	i = 0;
	while (i < 4)
	{
		if (board[row - i][col - i] != player)
			return (0);
		++i;
	}
	return (player);
}

int	check_win_up_right(int row, int col, int player)
{
	int	i;

	if (row < 3 || col > 3)
		return (0);
	i = 0;
	while (i < 4)
	{
		if (board[row - i][col + i] != player)
			return (0);
		++i;
	}
	return (player);
}

void	set_zero_board(void)
{
	int	row;
	int	col;

	row = 0;
	while (row < 6)
	{
		col = 0;
		while (col < 7)
		{
			board[row][col] = 0;
			++col;
		}
		++row;
	}
	ST = col0_p2;
	last_ST = col0_p2;
}
