USE [QPTreasureDB]
GO

/****** Object:  StoredProcedure [dbo].[NET_PW_Sign]    Script Date: 04/03/2015 17:30:12 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO




CREATE PROC [dbo].[NET_PW_Sign]
@UserID INT,
@BqNum INT, --补签第几天  默认值是0,代表不是补签
@Flag INT,  --0获取该玩家签到信息，1执行该玩家签到操作
@strErrorDescribe	NVARCHAR(127) OUTPUT	--输出信息	
AS
BEGIN

	DECLARE @Score INT,@Type INT,@SignDate DATETIME,@NUM INT,@Continuous INT,@GetDAY INT
	DECLARE @OneDay INT,@TwoDay INT,@ThreeDay INT,@FourDay INT,@FiveDay INT,@SixDay INT,@SevenDay INT,@AllDay INT,@BqScore INT
	DECLARE @Day1 INT,@Day2 INT,@Day3 INT,@Day4 INT,@Day5 INT,@Day6 INT,@Day7 INT
	SET @Day1=0; --0是未签到  1是签到
	SET @Day2=0;
	SET @Day3=0;
	SET @Day4=0;
	SET @Day5=0;
	SET @Day6=0;
	SET @Day7=0;
	
	--@Type签到类型（0按次数，1按天数，2补签）
	SELECT @Type=[Type],@OneDay=OneDay,@TwoDay=TwoDay,@ThreeDay=ThreeDay,@FourDay=FourDay,@FiveDay=FiveDay,@SixDay=SixDay,@SevenDay=SevenDay,@AllDay=AllDay,@BqScore=BqScore FROM QPTreasureDB.dbo.SignDay
	SELECT TOP 1 @Continuous=Continuous,@SignDate=SignDate FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID ORDER BY SignDate DESC
	
	IF(@Flag=0)
		BEGIN
			IF EXISTS (SELECT UserID FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID AND Continuous=1)
				BEGIN
					SET @Day1=1;
				END
			IF EXISTS (SELECT UserID FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID AND Continuous=2)
				BEGIN
					SET @Day2=1;
				END
			IF EXISTS (SELECT UserID FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID AND Continuous=3)
				BEGIN
					SET @Day3=1;
				END
			IF EXISTS (SELECT UserID FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID AND Continuous=4)
				BEGIN
					SET @Day4=1;
				END
			IF EXISTS (SELECT UserID FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID AND Continuous=5)
				BEGIN
					SET @Day5=1;
				END
			IF EXISTS (SELECT UserID FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID AND Continuous=6)
				BEGIN
					SET @Day6=1;
				END
			IF EXISTS (SELECT UserID FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID AND Continuous=7)
				BEGIN
					SET @Day7=1;
				END
		 
			IF(@Type=2) --包含补签
				BEGIN 
					--第一天是否签到,第二天是否签到,第三天是否签到,第四天是否签到,第五天是否签到,第六天是否签到,第七天是否签到,是否补签,第一天奖励,第二天奖励,第三天奖励,第四天奖励,第五天奖励,第六天奖励,第七天奖励,满七天赠送奖励,补签所需花费金币数
					SELECT @Day1 AS Day1,@Day2 AS Day2,@Day3 AS Day3,@Day4 AS Day4,@Day5 AS Day5,@Day6 AS Day6,@Day7 AS Day7,'2' AS IsBq,@OneDay AS OneDay,@TwoDay AS TwoDay,@ThreeDay AS ThreeDay,@FourDay AS FourDay,@FiveDay AS FiveDay,@SixDay AS SixDay,@SevenDay AS SevenDay,@AllDay AS AllDay,@BqScore AS BqScore, @Continuous AS Continuous
				END
			ELSE IF(@Type=1) --按天数
				BEGIN
					--IF(ISNULL(@Continuous,0)=0 OR ISNULL(DATEDIFF(DAY, CONVERT(NVARCHAR(10),@SignDate,120),CONVERT(NVARCHAR(10),GETDATE(),120)),0)=0 OR ISNULL(DATEDIFF(DAY, CONVERT(NVARCHAR(10),@SignDate,120),CONVERT(NVARCHAR(10),GETDATE(),120)),0)>=2)
					--	BEGIN
					--		SET @Day1=1;
					--	END ELSE
					--第一天是否签到,第二天是否签到,第三天是否签到,第四天是否签到,第五天是否签到,第六天是否签到,第七天是否签到,是否补签,第一天奖励,第二天奖励,第三天奖励,第四天奖励,第五天奖励,第六天奖励,第七天奖励,满七天赠送奖励,补签所需花费金币数
					SELECT @Day1 AS Day1,@Day2 AS Day2,@Day3 AS Day3,@Day4 AS Day4,@Day5 AS Day5,@Day6 AS Day6,@Day7 AS Day7,'1' AS IsBq,@OneDay AS OneDay,@TwoDay AS TwoDay,@ThreeDay AS ThreeDay,@FourDay AS FourDay,@FiveDay AS FiveDay,@SixDay AS SixDay,@SevenDay AS SevenDay,@AllDay AS AllDay,@BqScore AS BqScore
				END
			ELSE IF(@Type=0) --按次数
				BEGIN
					--第一天是否签到,第二天是否签到,第三天是否签到,第四天是否签到,第五天是否签到,第六天是否签到,第七天是否签到,是否补签,第一天奖励,第二天奖励,第三天奖励,第四天奖励,第五天奖励,第六天奖励,第七天奖励,满七天赠送奖励,补签所需花费金币数
					SELECT @Day1 AS Day1,@Day2 AS Day2,@Day3 AS Day3,@Day4 AS Day4,@Day5 AS Day5,@Day6 AS Day6,@Day7 AS Day7,'0' AS IsBq,@OneDay AS OneDay,@TwoDay AS TwoDay,@ThreeDay AS ThreeDay,@FourDay AS FourDay,@FiveDay AS FiveDay,@SixDay AS SixDay,@SevenDay AS SevenDay,@AllDay AS AllDay,@BqScore AS BqScore
					
					RETURN 22
				END
		END
	ELSE IF(@Flag=1)
		BEGIN
			IF(@Type=0)--按次数
				BEGIN
					IF EXISTS (SELECT UserID FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID AND CONVERT(NVARCHAR(10),SignDate,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
						BEGIN
							SET @strErrorDescribe=N'今日已签到！'
							
							RETURN 26
						END
					ELSE
						BEGIN
							IF(ISNULL(@Continuous,0)=0)
								BEGIN
									UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@OneDay,ALLScore=ALLScore+@OneDay WHERE UserID=@UserID
									INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@OneDay,GETDATE(),0,1,GETDATE())
									IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
										BEGIN
											UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@OneDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
										END
									ELSE
										BEGIN
											INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@OneDay,GETDATE())
										END
								END
							ELSE IF(ISNULL(@Continuous,0)=1)
								BEGIN
									UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@TwoDay,ALLScore=ALLScore+@TwoDay WHERE UserID=@UserID
									INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@TwoDay,GETDATE(),0,2,GETDATE())
									IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
										BEGIN
											UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@TwoDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
										END
									ELSE
										BEGIN
											INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@TwoDay,GETDATE())
										END
								END
							ELSE IF(ISNULL(@Continuous,0)=2)
								BEGIN
									UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@ThreeDay,ALLScore=ALLScore+@ThreeDay WHERE UserID=@UserID
									INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@ThreeDay,GETDATE(),0,3,GETDATE())
									IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
										BEGIN
											UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@ThreeDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
										END
									ELSE
										BEGIN
											INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@ThreeDay,GETDATE())
										END
								END
							ELSE IF(ISNULL(@Continuous,0)=3)
								BEGIN
									UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@FourDay,ALLScore=ALLScore+@FourDay WHERE UserID=@UserID
									INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@FourDay,GETDATE(),0,4,GETDATE())
									IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
										BEGIN
											UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@FourDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
										END
									ELSE
										BEGIN
											INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@FourDay,GETDATE())
										END
								END
							ELSE IF(ISNULL(@Continuous,0)=4)
								BEGIN
									UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@FiveDay,ALLScore=ALLScore+@FiveDay WHERE UserID=@UserID
									INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@FiveDay,GETDATE(),0,5,GETDATE())
									IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
										BEGIN
											UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@FiveDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
										END
									ELSE
										BEGIN
											INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@FiveDay,GETDATE())
										END
								END
							ELSE IF(ISNULL(@Continuous,0)=5)
								BEGIN
									UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@SixDay,ALLScore=ALLScore+@SixDay WHERE UserID=@UserID
									INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@SixDay,GETDATE(),0,6,GETDATE())
									IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
										BEGIN
											UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@SixDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
										END
									ELSE
										BEGIN
											INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@SixDay,GETDATE())
										END
								END
							ELSE IF(ISNULL(@Continuous,0)=6)
								BEGIN
									UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@SevenDay+@AllDay,ALLScore=ALLScore+@SevenDay+@AllDay WHERE UserID=@UserID
									INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@SevenDay+@AllDay,GETDATE(),0,7,GETDATE())
									IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
										BEGIN
											UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@SevenDay+@AllDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
										END
									ELSE
										BEGIN
											INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@SevenDay+@AllDay,GETDATE())
										END
								END
							ELSE IF(ISNULL(@Continuous,0)=7)
								BEGIN
									DELETE FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID
									UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@OneDay,ALLScore=ALLScore+@OneDay WHERE UserID=@UserID
									INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@OneDay,GETDATE(),0,1,GETDATE())
									IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
										BEGIN
											UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@OneDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
										END
									ELSE
										BEGIN
											INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@OneDay,GETDATE())
										END
								END
							
							SET @strErrorDescribe=N'签到成功！'
							
							RETURN 24
						END
				END
			ELSE IF(@Type=1)  --按天数
				BEGIN
					IF EXISTS (SELECT UserID FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID AND CONVERT(NVARCHAR(10),SignDate,120)=CONVERT(NVARCHAR(10),GETDATE(),120) AND SignType=0)
						BEGIN
							SET @strErrorDescribe=N'今日已签到！'
							
							RETURN 26
						END
					ELSE
						BEGIN
							IF(ISNULL(@Continuous,0)=0 OR ISNULL(DATEDIFF(DAY, CONVERT(NVARCHAR(10),@SignDate,120),CONVERT(NVARCHAR(10),GETDATE(),120)),0)=0 OR ISNULL(DATEDIFF(DAY, CONVERT(NVARCHAR(10),@SignDate,120),CONVERT(NVARCHAR(10),GETDATE(),120)),0)>=2)
								BEGIN
									DELETE FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID
									UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@OneDay,ALLScore=ALLScore+@OneDay WHERE UserID=@UserID
									INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@OneDay,GETDATE(),0,1,GETDATE())
									IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
										BEGIN
											UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@OneDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
										END
									ELSE
										BEGIN
											INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@OneDay,GETDATE())
										END
								END
							ELSE IF(ISNULL(@Continuous,0)=1 AND ISNULL(DATEDIFF(DAY, CONVERT(NVARCHAR(10),@SignDate,120),CONVERT(NVARCHAR(10),GETDATE(),120)),0)=1)
								BEGIN
									UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@TwoDay,ALLScore=ALLScore+@TwoDay WHERE UserID=@UserID
									INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@TwoDay,GETDATE(),0,2,GETDATE())
									IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
										BEGIN
											UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@TwoDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
										END
									ELSE
										BEGIN
											INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@TwoDay,GETDATE())
										END
								END
							ELSE IF(ISNULL(@Continuous,0)=2 AND ISNULL(DATEDIFF(DAY, CONVERT(NVARCHAR(10),@SignDate,120),CONVERT(NVARCHAR(10),GETDATE(),120)),0)=1)
								BEGIN
									UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@ThreeDay,ALLScore=ALLScore+@ThreeDay WHERE UserID=@UserID
									INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@ThreeDay,GETDATE(),0,3,GETDATE())
									IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
										BEGIN
											UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@ThreeDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
										END
									ELSE
										BEGIN
											INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@ThreeDay,GETDATE())
										END
								END
							ELSE IF(ISNULL(@Continuous,0)=3 AND ISNULL(DATEDIFF(DAY, CONVERT(NVARCHAR(10),@SignDate,120),CONVERT(NVARCHAR(10),GETDATE(),120)),0)=1)
								BEGIN
									UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@FourDay,ALLScore=ALLScore+@FourDay WHERE UserID=@UserID
									INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@FourDay,GETDATE(),0,4,GETDATE())
									IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
										BEGIN
											UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@FourDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
										END
									ELSE
										BEGIN
											INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@FourDay,GETDATE())
										END
								END
							ELSE IF(ISNULL(@Continuous,0)=4 AND ISNULL(DATEDIFF(DAY, CONVERT(NVARCHAR(10),@SignDate,120),CONVERT(NVARCHAR(10),GETDATE(),120)),0)=1)
								BEGIN
									UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@FiveDay,ALLScore=ALLScore+@FiveDay WHERE UserID=@UserID
									INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@FiveDay,GETDATE(),0,5,GETDATE())
									IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
										BEGIN
											UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@FiveDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
										END
									ELSE
										BEGIN
											INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@FiveDay,GETDATE())
										END
								END
							ELSE IF(ISNULL(@Continuous,0)=5 AND ISNULL(DATEDIFF(DAY, CONVERT(NVARCHAR(10),@SignDate,120),CONVERT(NVARCHAR(10),GETDATE(),120)),0)=1)
								BEGIN
									UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@SixDay,ALLScore=ALLScore+@SixDay WHERE UserID=@UserID
									INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@SixDay,GETDATE(),0,6,GETDATE())
									IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
										BEGIN
											UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@SixDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
										END
									ELSE
										BEGIN
											INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@SixDay,GETDATE())
										END
								END
							ELSE IF(ISNULL(@Continuous,0)=6 AND ISNULL(DATEDIFF(DAY, CONVERT(NVARCHAR(10),@SignDate,120),CONVERT(NVARCHAR(10),GETDATE(),120)),0)=1)
								BEGIN
									UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@SevenDay+@AllDay,ALLScore=ALLScore+@SevenDay+@AllDay WHERE UserID=@UserID
									INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@SevenDay+@AllDay,GETDATE(),0,7,GETDATE())
									IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
										BEGIN
											UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@SevenDay+@AllDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
										END
									ELSE
										BEGIN
											INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@SevenDay+@AllDay,GETDATE())
										END
								END
							ELSE IF(ISNULL(@Continuous,0)=7 AND ISNULL(DATEDIFF(DAY, CONVERT(NVARCHAR(10),@SignDate,120),CONVERT(NVARCHAR(10),GETDATE(),120)),0)=1)
								BEGIN
									DELETE FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID
									UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@OneDay,ALLScore=ALLScore+@OneDay WHERE UserID=@UserID
									INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@OneDay,GETDATE(),0,1,GETDATE())
									IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
										BEGIN
											UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@OneDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
										END
									ELSE
										BEGIN
											INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@OneDay,GETDATE())
										END
								END
							
							SET @strErrorDescribe=N'签到成功！'
							
							RETURN 24
						END
				END
			ELSE IF(@Type=2) --包含补签(必须七天删除一次数据)
				BEGIN
			
					IF(@BqNum=0)--不补签
						BEGIN
							IF EXISTS (SELECT UserID FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID AND CONVERT(NVARCHAR(10),SignDate,120)=CONVERT(NVARCHAR(10),GETDATE(),120) AND SignType=0)
								BEGIN
									SET @strErrorDescribe=N'今日已签到！'
									
									RETURN 26
								END
							ELSE
								BEGIN
									IF(ISNULL(@Continuous,0)=0)
										BEGIN
											UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@OneDay,ALLScore=ALLScore+@OneDay WHERE UserID=@UserID
											INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@OneDay,GETDATE(),0,1,GETDATE())
											IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
												BEGIN
													UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@OneDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
												END
											ELSE
												BEGIN
													INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@OneDay,GETDATE())
												END
											SET @strErrorDescribe=N'签到成功！'
											
											RETURN 24
										END
									ELSE
										BEGIN
											SELECT @SignDate=SignDate FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID AND Continuous=1
											SET @NUM=DATEDIFF(DAY, CONVERT(NVARCHAR(10),@SignDate,120),CONVERT(NVARCHAR(10),GETDATE(),120))+1;
											
											IF(@NUM=1)
												BEGIN
													UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@OneDay,ALLScore=ALLScore+@OneDay WHERE UserID=@UserID
													INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@OneDay,GETDATE(),0,1,GETDATE())
													IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
														BEGIN
															UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@OneDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
														END
													ELSE
														BEGIN
															INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@OneDay,GETDATE())
														END
												END
											ELSE IF(@NUM=2)
												BEGIN
													UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@TwoDay,ALLScore=ALLScore+@TwoDay WHERE UserID=@UserID
													INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@TwoDay,GETDATE(),0,2,GETDATE())
													IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
														BEGIN
															UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@TwoDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
														END
													ELSE
														BEGIN
															INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@TwoDay,GETDATE())
														END
												END
											ELSE IF(@NUM=3)
												BEGIN
													UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@ThreeDay,ALLScore=ALLScore+@ThreeDay WHERE UserID=@UserID
													INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@ThreeDay,GETDATE(),0,3,GETDATE())
													IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
														BEGIN
															UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@ThreeDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
														END
													ELSE
														BEGIN
															INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@ThreeDay,GETDATE())
														END
												END
											ELSE IF(@NUM=4)
												BEGIN
													UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@FourDay,ALLScore=ALLScore+@FourDay WHERE UserID=@UserID
													INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@FourDay,GETDATE(),0,4,GETDATE())
													IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
														BEGIN
															UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@FourDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
														END
													ELSE
														BEGIN
															INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@FourDay,GETDATE())
														END
												END
											ELSE IF(@NUM=5)
												BEGIN
													UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@FiveDay,ALLScore=ALLScore+@FiveDay WHERE UserID=@UserID
													INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@FiveDay,GETDATE(),0,5,GETDATE())
													IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
														BEGIN
															UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@FiveDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
														END
													ELSE
														BEGIN
															INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@FiveDay,GETDATE())
														END
												END
											ELSE IF(@NUM=6)
												BEGIN
													UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@SixDay,ALLScore=ALLScore+@SixDay WHERE UserID=@UserID
													INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@SixDay,GETDATE(),0,6,GETDATE())
													IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
														BEGIN
															UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@SixDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
														END
													ELSE
														BEGIN
															INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@SixDay,GETDATE())
														END
												END
											ELSE IF(@NUM=7)
												BEGIN
													IF(SELECT COUNT(*) FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID AND Continuous IN (1,2,3,4,5,6,7))=7
														BEGIN
															UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@SevenDay+@AllDay,ALLScore=ALLScore+@SevenDay+@AllDay WHERE UserID=@UserID
															INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@SevenDay+@AllDay,GETDATE(),0,7,GETDATE())
															IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
																BEGIN
																	UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@SevenDay+@AllDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
																END
															ELSE
																BEGIN
																	INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@SevenDay+@AllDay,GETDATE())
																END
														END
													ELSE
														BEGIN
															UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@SevenDay,ALLScore=ALLScore+@SevenDay WHERE UserID=@UserID 
															INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@SevenDay,GETDATE(),0,7,GETDATE())
															IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
																BEGIN
																	UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@SevenDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
																END
															ELSE
																BEGIN
																	INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@SevenDay,GETDATE())
																END
														END
												END
											ELSE IF(@NUM=8)
												BEGIN
													DELETE FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID
													UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@OneDay,ALLScore=ALLScore+@OneDay WHERE UserID=@UserID
													INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@OneDay,GETDATE(),0,1,GETDATE())
													IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
														BEGIN
															UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@OneDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
														END
													ELSE
														BEGIN
															INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@OneDay,GETDATE())
														END
												END
											
											SET @strErrorDescribe=N'签到成功！'
											
											RETURN 24
										END
								END
						END
					ELSE IF(@BqNum>0)  --补签
						BEGIN
							IF EXISTS (SELECT UserID FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID AND Continuous=@BqNum)
								BEGIN
									SET @strErrorDescribe=N'该日期已签到，无须补签！'
									
									RETURN 26
								END
							ELSE
								BEGIN
									IF(@BqNum=2)
										BEGIN
											UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@TwoDay-@BqScore,ALLScore=ALLScore+@TwoDay-@BqScore WHERE UserID=@UserID
											INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@TwoDay-@BqScore,GETDATE(),1,2,GETDATE())
											IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
												BEGIN
													UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@TwoDay-@BqScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
												END
											ELSE
												BEGIN
													INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@TwoDay-@BqScore,GETDATE())
												END
										END
									ELSE IF(@BqNum=3)
										BEGIN
											UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@ThreeDay-@BqScore,ALLScore=ALLScore+@ThreeDay-@BqScore WHERE UserID=@UserID
											INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@ThreeDay-@BqScore,GETDATE(),1,3,GETDATE())
											IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
												BEGIN
													UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@ThreeDay-@BqScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
												END
											ELSE
												BEGIN
													INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@ThreeDay-@BqScore,GETDATE())
												END
										END
									ELSE IF(@BqNum=4)
										BEGIN
											UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@FourDay-@BqScore,ALLScore=ALLScore+@FourDay-@BqScore WHERE UserID=@UserID
											INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@FourDay-@BqScore,GETDATE(),1,4,GETDATE())
											IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
												BEGIN
													UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@FourDay-@BqScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
												END
											ELSE
												BEGIN
													INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@FourDay-@BqScore,GETDATE())
												END
										END
									ELSE IF(@BqNum=5)
										BEGIN
											UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@FiveDay-@BqScore,ALLScore=ALLScore+@FiveDay-@BqScore WHERE UserID=@UserID
											INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@FiveDay-@BqScore,GETDATE(),1,5,GETDATE())
											IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
												BEGIN
													UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@FiveDay-@BqScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
												END
											ELSE
												BEGIN
													INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@FiveDay-@BqScore,GETDATE())
												END
										END
									ELSE IF(@BqNum=6)
										BEGIN
											UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@SixDay-@BqScore,ALLScore=ALLScore+@SixDay-@BqScore WHERE UserID=@UserID
											INSERT INTO QPTreasureDB.dbo.SignLog(UserID,Score,SignDate,SignType,Continuous,BSignDate) VALUES(@UserID,@SixDay-@BqScore,GETDATE(),1,6,GETDATE())
											IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
												BEGIN
													UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@SixDay-@BqScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
												END
											ELSE
												BEGIN
													INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@SixDay-@BqScore,GETDATE())
												END
										END
									IF(SELECT COUNT(*) FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID AND Continuous IN (1,2,3,4,5,6,7))=7
										BEGIN
											--DELETE FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID
											UPDATE QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@AllDay,ALLScore=ALLScore+@AllDay WHERE UserID=@UserID
											IF EXISTS (SELECT * FROM QPTreasureDB.dbo.SignScore WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120))
												BEGIN
													UPDATE QPTreasureDB.dbo.SignScore SET Score=Score+@AllDay WHERE CONVERT(NVARCHAR(10),AddTime,120)=CONVERT(NVARCHAR(10),GETDATE(),120)
												END
											ELSE
												BEGIN
													INSERT INTO QPTreasureDB.dbo.SignScore(Score,AddTime) VALUES(@AllDay,GETDATE())
												END
										END
									SET @strErrorDescribe=N'补签成功！'
									
									RETURN 27
								END
						END
				END
		END
END




	

GO

