package com.zap.main;

public class Card {
	public final int cid;
	public final String name;
	public final char border;
	public final char number;
	public final char suit;
	public final boolean zap;
	public final boolean missed;
	public final int life;
	public final boolean forceDiscard;
	public final int draw;
	public final boolean onePlayer;
	public final boolean allPlayers;
	public final boolean onePlayerReachable;
	public final int onePlayerFixed;
	public final String image;
	//space duel, indians, general store
	//space jail, dynamite, barrel, scope, mustang
	Card(int cid, String name, char border, char number, char suit, boolean zap,
			boolean missed, int life, boolean forceDiscard, int draw, boolean onePlayer,
			boolean allPlayers, boolean onePlayerReachable, int onePlayerFixed, String image) {
		this.cid = cid;
		this.name = name;
		this.border = border;
		this.number = number;
		this.suit = suit;
		this.zap = zap;
		this.missed = missed;
		this.life = life;
		this.forceDiscard = forceDiscard;
		this.draw = draw;
		this.onePlayer = onePlayer;
		this.allPlayers = allPlayers;
		this.onePlayerReachable = onePlayerReachable;
		this.onePlayerFixed = onePlayerFixed;
		this.image = image;
	}

	public boolean isGunCard() {
		if (border == 'L' && onePlayerFixed != 0) {
			return true;
		} else {
			return false;
        }
    }

    public boolean isBlue() {
        return border == 'L';
    }

    public boolean isRegular() {
        return border == 'R';
    }

    public boolean selfTarget() {
        return isGunCard() || isMissed() || isBeer() || isStageCoach()
                || isWellsFargo() || isBarrel() || isMustang() || isScope()
                || isDynamite();
    }

    public boolean allPlayersIncSelf() {
        return isSaloon() || isGeneralStore();
    }

    public boolean allPlayersNotSelf() {
        return isGatling() || isAliens();
    }

    public boolean onePlayerNotSelf() {
        return isBang() || isPanic() || isCatBalou() || isDuel() || isJail();
    }

    public boolean isBang() {
        return name.equals("Zap!");
    }

    public boolean isDuel() {
        return name.equals("Duel");
    }

    public boolean isAliens() {
        return name.equals("Aliens");
    }

    public boolean isGeneralStore() {
        return name.equals("General Store");
    }

    public boolean isMissed() {
        return name.equals("Missed");
    }

    public boolean isBeer() {
        return name.equals("Beer");
    }

    public boolean isStageCoach() {
        return name.equals("Stagecoach");
    }

    public boolean isWellsFargo() {
        return name.equals("Wells Fargo");
    }

    public boolean isBarrel() {
        return name.equals("Barrel");
    }

    public boolean isMustang() {
        return name.equals("Mustang");
    }

    public boolean isScope() {
        return name.equals("Scope");
    }

    public boolean isSaloon() {
        return name.equals("Saloon");
    }

    public boolean isDynamite() {
        return name.equals("Dynamite");
    }

    public boolean isGatling() {
        return name.equals("Gatling");
    }

    public boolean isPanic() {
        return name.equals("Panic");
    }

    public boolean isCatBalou() {
        return name.equals("Cat Balou");
    }

    public boolean isJail() {
        return name.equals("Space Jail");
    }
}
