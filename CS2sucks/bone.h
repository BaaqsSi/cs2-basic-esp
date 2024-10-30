	enum bones : int {
		head = 6,
		neck = 5,
		spine = 4,
		spine_1 = 2,
		left_shoulder = 8,
		left_arm = 9,
		left_hand = 11,
		cock = 0,
		right_shoulder = 13,
		right_arm = 14,
		right_hand = 16,
		left_hip = 22,
		left_knee = 23,
		left_feet = 24,
		right_hip = 25,
		right_knee = 26,
		right_feet = 27,

	};

	struct boneconnection {
		int bone1;
		int bone2;

		boneconnection(int b1, int b2) : bone1(b1) , bone2(b2) {}
	};

	boneconnection boneconnections[] = {
		boneconnection(6,5),
		boneconnection(5,4),
		boneconnection(4,0),
		boneconnection(4,8),
		boneconnection(8,9),
		boneconnection(9,11),
		boneconnection(4,13),
		boneconnection(13,14),
		boneconnection(14,16),
		boneconnection(4,2),
		boneconnection(0,22),
		boneconnection(0,25),
		boneconnection(22,23),
		boneconnection(23,24),
		boneconnection(25,26),
		boneconnection(26,27),
	};