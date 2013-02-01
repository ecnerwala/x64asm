/*
Copyright 2103 eric schkufza

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef X64ASM_SRC_REG_SET_H
#define X64ASM_SRC_REG_SET_H

#include <iostream>

#include "src/env_bits.h"
#include "src/env_reg.h"
#include "src/m.h"
#include "src/mm.h"
#include "src/r.h"
#include "src/sreg.h"
#include "src/st.h"
#include "src/xmm.h"
#include "src/ymm.h"

namespace x64asm {

/** A compact bit set representation for registers. */
class RegSet {
	private:
		enum class Mask : uint64_t {
			// Group 1
			LOW     = 0x0000000000000001,
			HIGH    = 0x0000000000010000,
			WORD    = 0x0000000000010001,
			DOUBLE  = 0x0000000100010001,
		 	QUAD    = 0x0001000100010001,

			// Group 2
			XMM     = 0x0000000000000001,
			YMM     = 0x0000000000010001,
			MM      = 0x0000000100000000,
			ST      = 0x0000010000000000,
			SREG    = 0x0001000000000000,
      DATA    = 0x0100000000000000,
      INSTR   = 0x0200000000000000,
      OPCODE  = 0x0400000000000000,
      RIP     = 0x0800000000000000,

			// Group 3
			EFLAG   = 0x0000000000000001,
			CONTROL = 0x0000000100000000,
			STATUS  = 0x0001000000000000,

			// Group 4
			TAG     = 0x0000000000000001,
			MXCSR   = 0x0000000000010000,

			// Top and Bottom
			EMPTY   = 0x0000000000000000,
			UNIV1   = 0xffffffffffffffff,
			UNIV2   = 0x0f3fffffffffffff,
			UNIV3   = 0xe7ff1a3f003f6fd5,
			UNIV4   = 0x00000000dfffaaaa,
		};

		inline RegSet(uint64_t g1, uint64_t g2, uint64_t g3, uint64_t g4)
				: group1_{g1}, group2_{g2}, group3_{g3}, group4_{g4} {
		}

		inline RegSet(Mask g1, Mask g2, Mask g3, Mask g4)
				: group1_{(uint64_t)g1}, group2_{(uint64_t)g2}, group3_{(uint64_t)g3},
	        group4_{(uint64_t)g4}	{
		}

	public:	
		// Static Constants
		static inline RegSet empty() {
			return RegSet(Mask::EMPTY, Mask::EMPTY, Mask::EMPTY, Mask::EMPTY);
		}

		static inline RegSet universe() {
			return RegSet(Mask::UNIV1, Mask::UNIV2, Mask::UNIV3, Mask::UNIV4); 
		}

		// Set Operators
		inline RegSet operator~() const {
			return RegSet(~group1_, ~group2_, ~group3_, ~group4_);
		}

		inline RegSet operator&(const RegSet& rhs) const {
			auto ret = *this;
			return ret &= rhs;
		}

		inline RegSet operator|(const RegSet& rhs) const {
			auto ret = *this;
			return ret |= rhs;
		}

		inline RegSet operator-(const RegSet& rhs) const {
			auto ret = *this;
			return ret -= rhs;
		}

		inline RegSet& operator&=(const RegSet& rhs) {
			group1_ &= rhs.group1_;
			group2_ &= rhs.group2_;
			group3_ &= rhs.group3_;
			group4_ &= rhs.group4_;
			return *this;
		}

		inline RegSet& operator|=(const RegSet& rhs) {
			group1_ |= rhs.group1_;
			group2_ |= rhs.group2_;
			group3_ |= rhs.group3_;
			group4_ |= rhs.group4_;
			return *this;
		}

		inline RegSet& operator-=(const RegSet& rhs) {
			group1_ &= ~rhs.group1_;
			group2_ &= ~rhs.group2_;
			group3_ &= ~rhs.group3_;
			group4_ &= ~rhs.group4_;
			return *this;
		}

		// Comparison Operators
		inline bool operator==(const RegSet& rhs) const {
			return group1_ == rhs.group1_ && group2_ == rhs.group2_ &&
				     group3_ == rhs.group3_ && group4_ == rhs.group4_;
		}

		inline bool operator!=(const RegSet& rhs) const {
			return !(*this == rhs);
		}

		// Element Operators
		inline RegSet operator+(Rl rhs) const {
			auto ret = *this;
			return ret += rhs;
		}

		inline RegSet operator+(Rh rhs) const {
			auto ret = *this;
			return ret += rhs;
		}

		inline RegSet operator+(Rb rhs) const {
			auto ret = *this;
			return ret += rhs;
		}

		inline RegSet operator+(R16 rhs) const {
			auto ret = *this;
			return ret += rhs;
		}

		inline RegSet operator+(R32 rhs) const {
			auto ret = *this;
			return ret += rhs;
		}

		inline RegSet operator+(R64 rhs) const {
			auto ret = *this;
			return ret += rhs;
		}

		inline RegSet operator+(Xmm rhs) const {
			auto ret = *this;
			return ret += rhs;
		}

		inline RegSet operator+(Ymm rhs) const {
			auto ret = *this;
			return ret += rhs;
		}

		inline RegSet operator+(Mm rhs) const {
			auto ret = *this;
			return ret += rhs;
		}

		inline RegSet operator+(St rhs) const {
			auto ret = *this;
			return ret += rhs;
		}

		inline RegSet operator+(Sreg rhs) const {
			auto ret = *this;
			return ret += rhs;
		}

		inline RegSet operator+(FpuData rhs) const {
			auto ret = *this;
			return ret += rhs;
		}

		inline RegSet operator+(FpuInstruction rhs) const {
			auto ret = *this;
			return ret += rhs;
		}

		inline RegSet operator+(FpuOpcode rhs) const {
			auto ret = *this;
			return ret += rhs;
		}

		inline RegSet operator+(Rip rhs) const {
			auto ret = *this;
			return ret += rhs;
		}

		inline RegSet operator+(Eflags rhs) const {
			auto ret = *this;
			return ret += rhs;
		}

		inline RegSet operator+(FpuControl rhs) const {
			auto ret = *this;
			return ret += rhs;
		}

		inline RegSet operator+(FpuStatus rhs) const {
			auto ret = *this;
			return ret += rhs;
		}

		inline RegSet operator+(FpuTag rhs) const {
			auto ret = *this;
			return ret += rhs;
		}

		inline RegSet operator+(Mxcsr rhs) const {
			auto ret = *this;
			return ret += rhs;
		}

		inline RegSet operator+(const M& rhs) const {
			auto ret = *this;
			return ret += rhs;
		}

		inline RegSet& operator+=(Rl rhs) {
			group1_ |= ((uint64_t) Mask::LOW << rhs.val_);
			return *this;
		}

		inline RegSet& operator+=(Rh rhs) {
			group1_ |= ((uint64_t) Mask::HIGH << (rhs.val_-4));
			return *this;
		}

		inline RegSet& operator+=(Rb rhs) {
			group1_ |= ((uint64_t) Mask::LOW << rhs.val_);
			return *this;
		}

		inline RegSet& operator+=(R16 rhs) {
			group1_ |= ((uint64_t) Mask::WORD << rhs.val_);
			return *this;
		}

		inline RegSet& operator+=(R32 rhs) {
			group1_ |= ((uint64_t) Mask::DOUBLE << rhs.val_);
			return *this;
		}

		inline RegSet& operator+=(R64 rhs) {
			group1_ |= ((uint64_t) Mask::QUAD << rhs.val_);
			return *this;
		}

		inline RegSet& operator+=(Xmm rhs) {
			group2_ |= ((uint64_t) Mask::XMM << rhs.val_);
			return *this;
		}

		inline RegSet& operator+=(Ymm rhs) {
			group2_ |= ((uint64_t) Mask::YMM << rhs.val_);
			return *this;
		}

		inline RegSet& operator+=(Mm rhs) {
			group2_ |= ((uint64_t) Mask::MM << rhs.val_);
			return *this;
		}

		inline RegSet& operator+=(St rhs) {
			group2_ |= ((uint64_t) Mask::ST << rhs.val_);
			return *this;
		}

		inline RegSet& operator+=(Sreg rhs) {
			group2_ |= ((uint64_t) Mask::SREG << rhs.val_);
			return *this;
		}

		inline RegSet& operator+=(FpuData rhs) {
			group2_ |= ((uint64_t) Mask::DATA << rhs.val_);
			return *this;
		}

		inline RegSet& operator+=(FpuInstruction rhs) {
			group2_ |= ((uint64_t) Mask::INSTR << rhs.val_);
			return *this;
		}

		inline RegSet& operator+=(FpuOpcode rhs) {
			group2_ |= ((uint64_t) Mask::OPCODE << rhs.val_);
			return *this;
		}

		inline RegSet& operator+=(Rip rhs) {
			group2_ |= ((uint64_t) Mask::RIP << rhs.val_);
			return *this;
		}

		inline RegSet& operator+=(Eflags rhs) {
			group3_ |= ((uint64_t) Mask::EFLAG << rhs.index());
			return *this;
		}

		inline RegSet& operator+=(FpuControl rhs) {
			group3_ |= ((uint64_t) Mask::CONTROL << rhs.index());
			return *this;
		}

		inline RegSet& operator+=(FpuStatus rhs) {
			group3_ |= ((uint64_t) Mask::STATUS << rhs.index());
			return *this;
		}

		inline RegSet& operator+=(FpuTag rhs) {
			group4_ |= ((uint64_t) Mask::TAG << rhs.index());
			return *this;
		}

		inline RegSet& operator+=(Mxcsr rhs) {
			group4_ |= ((uint64_t) Mask::MXCSR << rhs.index());
			return *this;
		}

		RegSet& operator+=(const M& rhs);

		// Queries

		inline bool contains(Rl rhs) const {
			return ((group1_ >> rhs.val_) & (uint64_t)Mask::LOW) == 
				     (uint64_t)Mask::LOW;
		}

		inline bool contains(Rh rhs) const {
			return ((group1_ >> (rhs.val_-4)) & (uint64_t)Mask::HIGH) == 
				     (uint64_t)Mask::HIGH;
		}

		inline bool contains(Rb rhs) const {
			return ((group1_ >> rhs.val_) & (uint64_t)Mask::LOW) == 
				     (uint64_t)Mask::LOW;
		}

		inline bool contains(R16 rhs) const {
			return ((group1_ >> rhs.val_) & (uint64_t)Mask::WORD) == 
				     (uint64_t)Mask::WORD;
		}

		inline bool contains(R32 rhs) const {
			return ((group1_ >> rhs.val_) & (uint64_t)Mask::DOUBLE) == 
				     (uint64_t)Mask::DOUBLE;
		}

		inline bool contains(R64 rhs) const {
			return ((group1_ >> rhs.val_) & (uint64_t)Mask::QUAD) == 
				     (uint64_t)Mask::QUAD;
		}

		inline bool contains(Xmm rhs) const {
			return ((group2_ >> rhs.val_) & (uint64_t)Mask::XMM) == 
				     (uint64_t)Mask::XMM;
		}

		inline bool contains(Ymm rhs) const {
			return ((group2_ >> rhs.val_) & (uint64_t)Mask::YMM) == 
				     (uint64_t)Mask::YMM;
		}

		inline bool contains(Mm rhs) const {
			return ((group2_ >> rhs.val_) & (uint64_t)Mask::MM) == 
				     (uint64_t)Mask::MM;
		}

		inline bool contains(St rhs) const {
			return ((group2_ >> rhs.val_) & (uint64_t)Mask::ST) == 
				     (uint64_t)Mask::ST;
		}

		inline bool contains(Sreg rhs) const {
			return ((group2_ >> rhs.val_) & (uint64_t)Mask::SREG) == 
				     (uint64_t)Mask::SREG;
		}

		inline bool contains(FpuData rhs) const {
			return ((group2_ >> rhs.val_) & (uint64_t)Mask::DATA) == 
				     (uint64_t)Mask::DATA;
		}

		inline bool contains(FpuInstruction rhs) const {
			return ((group2_ >> rhs.val_) & (uint64_t)Mask::INSTR) == 
				     (uint64_t)Mask::INSTR;
		}

		inline bool contains(FpuOpcode rhs) const {
			return ((group2_ >> rhs.val_) & (uint64_t)Mask::OPCODE) == 
				     (uint64_t)Mask::OPCODE;
		}

		inline bool contains(Rip rhs) const {
			return ((group2_ >> rhs.val_) & (uint64_t)Mask::RIP) == 
				     (uint64_t)Mask::RIP;
		}

		inline bool contains(Eflags rhs) const {
			return ((group3_ >> rhs.index()) & (uint64_t)Mask::EFLAG) == 
				     (uint64_t)Mask::EFLAG;
		}

		inline bool contains(FpuControl rhs) const {
			return ((group3_ >> rhs.index()) & (uint64_t)Mask::CONTROL) == 
				     (uint64_t)Mask::CONTROL;
		}

		inline bool contains(FpuStatus rhs) const {
			return ((group3_ >> rhs.index()) & (uint64_t)Mask::STATUS) == 
				     (uint64_t)Mask::STATUS;
		}

		inline bool contains(FpuTag rhs) const {
			return ((group4_ >> rhs.index()) & (uint64_t)Mask::TAG) == 
				     (uint64_t)Mask::TAG;
		}

		inline bool contains(Mxcsr rhs) const {
			return ((group4_ >> rhs.index()) & (uint64_t)Mask::MXCSR) == 
				     (uint64_t)Mask::MXCSR;
		}

	private:
		uint64_t group1_;
		uint64_t group2_;
		uint64_t group3_;
		uint64_t group4_;
};

} // namespace x64asm

#endif