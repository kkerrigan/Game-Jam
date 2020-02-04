--create the master command group that 
--will hold all the commands/command groups
createCommandGroup("MasterGroup", false)

	--sub group for the lead plane
	createCommandGroup("LeadPlane", true)
	addCommandGroupToGroup("LeadPlane","MasterGroup")
	
	createCommandGroup("RotateForTakeoff", false)
	addCommandGroupToGroup("RotateForTakeoff","LeadPlane")
		orientToEntity(
			"RotateForTakeoff",	--command group
			3,					--entity to rotate
			6,					--target entity
			5,					--time
			1,					--ease in
			1					--ease out
		)
	
	createCommandGroup("StartTakeoffRoll", false)
	addCommandGroupToGroup("StartTakeoffRoll","LeadPlane")
		moveToTime(
			"StartTakeoffRoll", --command group
			3, 					--entity to move
			100, 1, -20, 		--position to move to 
			8, 					--duration
			5.0,				--ease in 
			0					--ease out
		)
		
	createCommandGroup("RotateAndMoveToRect", false)
	addCommandGroupToGroup("RotateAndMoveToRect","LeadPlane")
		orientToEntityDistance(
			"RotateAndMoveToRect",	--command group
			3,						--entity to rotate
			4,						--target entity
			0.5,					--time
			0,						--ease in
			0						--ease out
		)
		moveToTime(
			"RotateAndMoveToRect", 	--command group
			3,                      --entity to move
			0, 15, -20,             --position to move to
			10,                     --duration
			0.0,                    --ease in 
			0.0                     --ease out
		)
		
	createCommandGroup("CruiseToDest", false)
	addCommandGroupToGroup("CruiseToDest","LeadPlane")
		orientToEntity(
			"CruiseToDest",			--command group
			3,						--entity to rotate
			7,						--target entity
			5,						--time
			0,						--ease in
			3						--ease out
		)
		moveToTime(
			"CruiseToDest", 		--command group
			3,                      --entity to move
			-80, 5, -40,            --position to move to
			30,                     --duration
			0,                      --ease in 
			5.0                     --ease out
		)
		
	createCommandGroup("LoopBack",false)
	addCommandGroupToGroup("LoopBack","LeadPlane")
		addCommandGroupToGroup("LoopBack","LeadPlane")
		entityFollowCurve(
			"LoopBack", 	--Command group
			3, 				--entity
			-120, 85, -40,	--destination 
			10,				--time 
			-120, 5, -50.0,	--control point
			0.1,			--easeIn
			0.1				--easeOut
		)
		
	createCommandGroup("LoopBack2",false)
	addCommandGroupToGroup("LoopBack2","LeadPlane")
		addCommandGroupToGroup("LoopBack2","LeadPlane")
		entityFollowCurve(
			"LoopBack2", 		--Command group
			3, 					--entity
			-80, 120, -40,		--destination 
			10,					--time 
			-120, 125, -50.0,	--control point
			0.05,				--easeIn
			0.05				--easeOut
		)
		
	createCommandGroup("LoopBack3",false)
	addCommandGroupToGroup("LoopBack3","LeadPlane")
		addCommandGroupToGroup("LoopBack3","LeadPlane")
		entityFollowCurve(
			"LoopBack3", 		--Command group
			3, 					--entity
			-40, 80, -40,		--destination 
			10,					--time 
			-40, 125, -40.0,	--control point
			0.05,				--easeIn
			0.05				--easeOut
		)
		
	createCommandGroup("LoopBack4",false)
	addCommandGroupToGroup("LoopBack4","LeadPlane")
		addCommandGroupToGroup("LoopBack4","LeadPlane")
		entityFollowCurve(
			"LoopBack4", 	--Command group
			3, 				--entity
			0, 40, -40,		--destination 
			10,				--time 
			-40, 40, -40.0,	--control point
			0.05,			--easeIn
			0.05			--easeOut
		)
		
	createCommandGroup("LoopBack5",false)
	addCommandGroupToGroup("LoopBack5","LeadPlane")
		addCommandGroupToGroup("LoopBack5","LeadPlane")
		entityFollowCurve(
			"LoopBack5", 	--Command group
			3, 				--entity
			40, 20, -20,	--destination 
			10,				--time 
			20, 30, -40.0,	--control point
			0.05,			--easeIn
			0.05			--easeOut
		)
		
	createCommandGroup("LoopBack6",false)
	addCommandGroupToGroup("LoopBack6","LeadPlane")
		addCommandGroupToGroup("LoopBack6","LeadPlane")
		entityFollowCurve(
			"LoopBack6", 	--Command group
			3, 				--entity
			40, 1, 40,		--destination 
			10,				--time 
			40, 1, 0.0,		--control point
			0.05,			--easeIn
			0.2				--easeOut
		)
		
	createCommandGroup("LoopBack7",false)
	addCommandGroupToGroup("LoopBack7","LeadPlane")
		addCommandGroupToGroup("LoopBack7","LeadPlane")
		entityFollowCurve(
			"LoopBack7", 	--Command group
			8, 				--entity
			40, 1, 70,		--destination 
			10,				--time 
			40, 1, 65.0,	--control point
			0.05,			--easeIn
			0.2				--easeOut
		)
		
	--have plane a follow the lead plane 
	createCommandGroup("FollowPlaneA", true)
	addCommandGroupToGroup("FollowPlaneA","MasterGroup")
		followEntity(
			"FollowPlaneA", --command group
			8,				--entity to move
			3,              --entity to follow
			1.0,1.0,1.0,    --follow distance
			10.0,           --min distance
			50.0,           --max speed distance
			10.0,           --max speed
			10.0,-1.0,10.0, --offset
			10.0,            --ease in
			10.0             --ease out
		)
		
	--have plane b follow the lead plane
	createCommandGroup("FollowPlaneB", true)
	addCommandGroupToGroup("FollowPlaneB","MasterGroup")
		followEntity(
			"FollowPlaneB",	--command group
			9,              --entity to move
			3,              --entity to follow
			1.0,1.0,1.0,    --follow distance
			10.0,           --min distance
			50.0,           --max speed distance
			10.0,           --max speed
			10.0,-1.0,-10.0,--offset
			10.0,            --ease in
			10.0             --ease out
		)
	
	--add a location based trigger that creates commands when triggered
	addTrigger(
		"MasterGroup",				--command group
		7, 							--entity
		1.0, 0.0, 0.0,				--color 
		1.0, 						--fade duration
		5.0, 						--collision radius
		"cFadeEntityColorTrigger" 	--command group to execute
	)
	
	--move the camera around the scene
	createCommandGroup("MoveCamera", false)
	addCommandGroupToGroup("MoveCamera", "MasterGroup")
		cameraFollowEntity(
			"MoveCamera", 	-- command group
			3,				-- Entity to follow
			1.0,1.0,1.0, 	-- follow distance
			5.0,			-- min distance
			50.0,			-- max speed distance
			50.0,			-- max speed
			50.0,5.0,-10.0,	-- offset
			4.0,			-- easeIn
			4.0,			-- easeOut
			35.0			-- time to follow for
		)
		
	createCommandGroup("MoveCamera1", false)
	addCommandGroupToGroup("MoveCamera1", "MasterGroup")
		cameraMoveToTime(
			"MoveCamera1", 	-- command group
			200, 200, -50,	-- position	
			15,				-- time
			2.0,			-- easeIn
			2.0				-- easeOut
		)
		
	createCommandGroup("MoveCamera2", false)
	addCommandGroupToGroup("MoveCamera2", "MasterGroup")
		cameraMoveToTime(
			"MoveCamera2", 	-- command group
			200, 40, -50,	-- position	
			20,				-- time
			3.0,			-- easeIn
			3.0				-- easeOut
		)
		
	createCommandGroup("MoveCamera3", false)
	addCommandGroupToGroup("MoveCamera3", "MasterGroup")
		cameraMoveToTime(
			"MoveCamera3", 	-- command group
			200, 0, -50,	-- position	
			8,				-- time
			2.0,			-- easeIn
			2.0				-- easeOut
		)
		
	createCommandGroup("MoveCamera4", false)
	addCommandGroupToGroup("MoveCamera4", "MasterGroup")
		cameraMoveToTime(
			"MoveCamera4", 	-- command group
			200, 20, 0,		-- position	
			10,				-- time
			2.0,			-- easeIn
			2.0				-- easeOut
		)
		
			
		