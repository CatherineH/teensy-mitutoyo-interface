#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Provides the support for the Mitutoyo interface.

Class originally contributed by Catherine Holloway.
"""

# IMPORTS #####################################################################

from __future__ import absolute_import
from __future__ import division

import quantities as pq
from instruments.abstract_instruments import Instrument

# CLASSES #####################################################################


class MitutoyoInterface(Instrument):
    """
    The Mitutoyo Interface is a low-cost teensy-based interface to a mitutoyo
    indicator.
    """
    def __init__(self, filelike):
        super(MitutoyoInterface, self).__init__(filelike)
        self.terminator = "\r"

    @property
    def decimal(self):
        """
        Get the decimal reading off of the indicator.

        :type: int
        """
        return self.query("DECI?")

    @property
    def good_reading(self):
        """
        Get the current reading state.
        :rtype: int
        """
        return int(self.query("GOOD?"))

    @property
    def raw(self):
        """
        Get the raw value from the dial indicator
        :rtype: str
        """
        query = self.query("RAWD?")
        return query

    @property
    def reading(self):
        """
        Get the current reading off of the indicator.

        :type: `~quantities.Quantity`
        :units: millimeters or mils
        """
        query = self.query("READ?").split(" ")
        response = pq.Quantity(float(query[0]), query[1])
        return response

    @property
    def sign(self):
        """
        Get the sign reading off of the indicator. 1 means positive and 0
        means negative.

        :type: int
        """
        return self.query("SIGN?")

    @property
    def units(self):
        """
        Get the unit reading off of the indicator. 0 means mm and 1 means
        thousandths of an inch.

        :type: int
        """
        return self.query("UNIT?")


if __name__ == "__main__":
    mi = MitutoyoInterface.open_serial(vid=5824, pid=1155, serial_number='1743330', baud=9600)
    while True:
       print(mi.reading, mi.raw, mi.good_reading)
