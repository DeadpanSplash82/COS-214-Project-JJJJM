
#ifndef __Battalion_h__
#define __Battalion_h__


class Battalion
{
	public:
		/**
		 * @brief construct a Battalion object
		 * */
		Battalion();
		/**
		 * @brief destroys a Battallion object
		 **/
		~Battalion();

		/**
		 * @brief attack method towards the enemy Country
		 * @param Country*- Country reference object
		 **/
		void attack(Country* enemy);
		/**
		 * @brief converts Citizens into battalions
		 * @param Citizen object- should be the population
		 **/
		void addBattalions(Citizen *citiz);
		/**
		 * @brief sets number of deceased battalions in battle
		 **/
		void decreaseBattalions(int deceased);
		/**
		 * @brief sets the number Battalions an attack of a battallion kills.
		 * @param int of the number of Battalions
		 **/
		void setNumBattalionDestroys(int);
		/**
		 * @brief retrieve number of battallions the battablion can destroy
		 * @return int - number of Battalions
		 **/
		int getBattalionDestroyed();
	private:
	int numBattalionDestroys;
		
};

#endif