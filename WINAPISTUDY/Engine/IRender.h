#pragma once

class IRender abstract
{
public:
	virtual void Render() PURE;

	int GetZOrder() const { return m_ZOrder; }
	void SetZOrder(int ZOrder) { m_ZOrder = ZOrder; }

protected:
	int m_ZOrder;
};
