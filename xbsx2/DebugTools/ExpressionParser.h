/*  XBSX2 - PS2 Emulator for Xbox Consoles  
 *  Copyright (C) 2002-2014  PCSX2 Dev Team
 * 
 *  XBSX2 is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU Lesser General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  XBSX2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with XBSX2.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "common/Xbsx2Types.h"
#include <vector>

typedef std::pair<u64,u64> ExpressionPair;
typedef std::vector<ExpressionPair> PostfixExpression;

enum ExpressionType
{
	EXPR_TYPE_UINT = 0,
	EXPR_TYPE_FLOAT = 2,
};

class IExpressionFunctions
{
public:
	virtual bool parseReference(char* str, u64& referenceIndex) = 0;
	virtual bool parseSymbol(char* str, u64& symbolValue) = 0;
	virtual u64 getReferenceValue(u64 referenceIndex) = 0;
	virtual ExpressionType getReferenceType(u64 referenceIndex) = 0;
	virtual bool getMemoryValue(u32 address, int size, u64& dest, char* error) = 0;
};

bool initPostfixExpression(const char* infix, IExpressionFunctions* funcs, PostfixExpression& dest);
bool parsePostfixExpression(PostfixExpression& exp, IExpressionFunctions* funcs, u64& dest);
bool parseExpression(const char* exp, IExpressionFunctions* funcs, u64& dest);
const char* getExpressionError();
