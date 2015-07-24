!-------------------------------------------------------------------------------
! Name: ncdf_read_field.inc
!
! Purpose:
! Code shared by all version of nc_read_array function. See orac_ncdf.F90 for
! detailed header information.
!
! Description and Algorithm details:
! 1) Locate variable in file
! 2) Read fill values, apply scale factor, add offset
!
! History:
! 2014/02/10, AP: Original version.
! 2014/08/15, AP: Fixed bug in management of scale factor/offset. Homogenizing
!   verbose and error printing formats. Split into open field/read field.
! 2014/09/03, GM: Fixed bug: fv should not be used as a loop local auxillary
!   variable.
!
! $Id: nc_read_field.inc 2726 2014-10-15 14:51:55Z gmcgarragh $
!
! Bugs:
! None known.
!-------------------------------------------------------------------------------

   if (verbose) print*,'Reading variable: ',trim(name)

   ! read data
   ierr = nf90_get_var(ncid, vid, arr_packed, start, counter, stride)
   if (ierr.ne.NF90_NOERR) then
      print*,'ERROR: nc_read_file(): Could not read variable ',trim(name)
      print*,trim(nc_error(ierr))
      stop error_stop_code
   endif

   ! replace file's fill value with our own and apply scale factor/offset
   if (fv_flag) &
      where (arr_packed.eq.fv) arr = fill
   if (vr_flag) then
      where (arr_packed.lt.vmin) arr = fill
      where (arr_packed.gt.vmax) arr = fill
   endif
   where (arr_packed.ne.fv) arr = sf*arr_packed + of

   ! additional information for print out
   if (verbose) then
      if (nf90_get_att(ncid,vid,'units',unit).eq.NF90_NOERR) &
           print*,'Field units: ',trim(unit)

      if (vr_flag) then
           print*,'Field valid min: ',sf*vmin + of
           print*,'Field valid max: ',sf*vmax + of
      endif
   end if